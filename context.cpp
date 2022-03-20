#include "pch.h"
#include "context.h"
#include "task_load_regions.h"

//---------------------------------------------------------------------------------------------------------
context& ctx()
{
	static context instance;
	return instance;
}

//---------------------------------------------------------------------------------------------------------
context::context()
	: multi_handle_{make_shared<curl_multi>()}
	, db_{"postgresql://eve:eve_pas@localhost:5432/eve"}
{}
//---------------------------------------------------------------------------------------------------------
const string& context::esi_subdir() const noexcept
{
	static string str{"https://esi.evetech.net/dev"};
	return str;
}
//---------------------------------------------------------------------------------------------------------
const string& context::esi_datasource() const noexcept
{
	static string str{"tranquility"};
	return str;
}
//---------------------------------------------------------------------------------------------------------
void context::add_task(shared_ptr<task> task)
{
	tasks_.emplace(task->handle(), task);
	task->activate(multi_handle_);
}
//---------------------------------------------------------------------------------------------------------
void context::_clear()
{
	region_ids_.clear();
	anomaly_sensor_.clear();
}
//---------------------------------------------------------------------------------------------------------
void context::_run()
{
	add_task(make_shared<task_load_regions>(1));

	while (!tasks_.empty())
	{
		int still_alive{0};
		curl_multi_perform(multi_handle_->handle(), &still_alive);

		int msgs_left{0};
		while (CURLMsg* msg{curl_multi_info_read(multi_handle_->handle(), &msgs_left)})
		{
			if (msg->msg == CURLMSG_DONE)
			{
				const auto task_i{tasks_.find(msg->easy_handle)};
				if (task_i == tasks_.cend())
				{
					cout << "no tasks found";
				}
				else
				{
					shared_ptr<task> next_task{task_i->second->finish()};
					tasks_.erase(task_i);
					if (next_task)
					{
						add_task(move(next_task));
					}
				}
			}
		}
        if (!tasks_.empty())
        {
            curl_multi_wait(multi_handle_->handle(), nullptr, 0, 1000, nullptr);
        }
	}
	cout << "done" << endl;
}
//---------------------------------------------------------------------------------------------------------
void context::start()
{
	_clear();
	_run();
}
//---------------------------------------------------------------------------------------------------------
void context::set_type(universe::type&& t)
{
	type_dict_.emplace(t.type_id_, t);
}
//---------------------------------------------------------------------------------------------------------
const universe::type& context::type_by_id(long long type_id) const noexcept
{
	static const universe::type null{};
	const auto type_i{type_dict_.find(type_id)};
	if (type_i == type_dict_.cend())
	{
		return null;
	}
	return type_i->second;
}
//---------------------------------------------------------------------------------------------------------
void context::set_system(universe::system&& t)
{
	system_dict_.emplace(t.system_id_, t);
}
//---------------------------------------------------------------------------------------------------------
const universe::system& context::system_by_id(long long system_id) const noexcept
{
	static const universe::system null{};
	const auto system_i{system_dict_.find(system_id)};
	if (system_i == system_dict_.cend())
	{
		return null;
	}
	return system_i->second;
}
//---------------------------------------------------------------------------------------------------------
void context::add_region_ids(const vector<long long>& ids)
{
	region_ids_.insert(region_ids_.end(), ids.begin(), ids.end());
}
//---------------------------------------------------------------------------------------------------------
void context::set_region_has_market(long long region_id, bool has)
{
	if (has)
	{
		region_no_market_.erase(region_id);
	}
	else
	{
		region_no_market_.emplace(region_id, 0).first->second++;
	}
}
//---------------------------------------------------------------------------------------------------------
bool context::region_has_market(long long region_id) const noexcept
{
	constexpr int threshold{1};
	const auto iter{region_no_market_.find(region_id)};
	if (iter != region_no_market_.cend())
	{
		return iter->second < threshold;
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------
void context::apply_orders(long long region_id, vector<order>&& orders)
{
	//printf("orders from region %lld. count %lu\n", region_id, orders.size());
	anomaly_sensor_.apply_orders(region_id, orders);
}
