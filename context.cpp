#include <iostream>
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
{}
//---------------------------------------------------------------------------------------------------------
void context::add_task(shared_ptr<task> task)
{
	tasks_.emplace(task->handle(), task);
	task->activate(multi_handle_);
}
//---------------------------------------------------------------------------------------------------------
void context::_run()
{
	add_task(make_shared<task_load_regions>());

	while (!tasks_.empty())
	{
		int still_alive{0};
		const CURLMcode res{curl_multi_perform(multi_handle_->handle(), &still_alive)};
		cout << "still_alive: " << still_alive << " res: " << res << endl;

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
					task_i->second->finish();
					tasks_.erase(task_i);
				}
				cout << "done" << endl;
			}
		}
        if (still_alive)
        {
            curl_multi_wait(multi_handle_->handle(), nullptr, 0, 1000, nullptr);
        }
    }
}
//---------------------------------------------------------------------------------------------------------
void context::start()
{
	_run();
}
//---------------------------------------------------------------------------------------------------------
void context::apply_orders(long long region_id, vector<order>&& orders)
{
	printf("orders from region %lld. count %lu\n", region_id, orders.size());
}
