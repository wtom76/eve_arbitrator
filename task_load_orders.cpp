#include "pch.h"
#include "task_load_orders.h"
#include "context.h"
#include "order.h"

//---------------------------------------------------------------------------------------------------------
task_load_orders::task_load_orders(long long region_idx, int page)
	: task{page}
	, region_idx_{region_idx}
{
	assert(region_idx_ >= 0);
	assert(static_cast<size_t>(region_idx_) < ctx().region_ids().size());

	const string url{ctx().esi_subdir() + "/markets/" + to_string(region_id()) + "/orders/?datasource=" + ctx().esi_datasource() + "&order_type=all&page=" + to_string(page_)};
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());

	{
		ofstream f{"dumps/orders.dump", ios::binary|ios::trunc};
		f.close();
	}
}
//---------------------------------------------------------------------------------------------------------
long long task_load_orders::region_id() const noexcept
{
	return ctx().region_ids()[region_idx_];
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_orders::_apply_raw_data(vector<char>&& data)
{
	{
		ofstream f{"dumps/orders.dump", ios::binary|ios::app};
		f.write(data.data(), data.size());
		f.close();
	}

	const bool empty_page{_is_page_empty(data)};

	if (!empty_page)
	{
		try
		{
			ctx().set_region_has_market(region_id(), true);
			ctx().apply_orders(region_id(), nlohmann::json::parse(data.data(), next(data.data(), data.size())).get<vector<order>>());
		}
		catch (const nlohmann::detail::exception& ex)
		{
			cout << "JSON ORDERS ERROR. region_id: " << region_id() << "\n" << ex.what() << endl;
		}
	}
	else
	{
		ctx().set_region_has_market(region_id(), false);
		//cout << "region " << region_id() << " has no market" << endl;
	}

	if (page_ < pages_ && !empty_page)
	{
		return make_shared<task_load_orders>(region_idx_, page_ + 1);
	}

	long long new_region_idx{region_idx_ + 1};
	while (static_cast<size_t>(new_region_idx) < ctx().region_ids().size() && !ctx().region_has_market(ctx().region_ids()[new_region_idx]))
	{
		++new_region_idx;
	}
	
	if (static_cast<size_t>(new_region_idx) < ctx().region_ids().size())
	{
		return make_shared<task_load_orders>(new_region_idx, 1);
	}

	ctx().clear_orders();

	new_region_idx = 0;
	while (static_cast<size_t>(new_region_idx) < ctx().region_ids().size() && !ctx().region_has_market(ctx().region_ids()[new_region_idx]))
	{
		++new_region_idx;
	}

	if (static_cast<size_t>(new_region_idx) < ctx().region_ids().size())
	{
		return make_shared<task_load_orders>(new_region_idx, 1);
	}

	return {};
}
