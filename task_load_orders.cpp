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
			ctx().apply_orders(region_id(), nlohmann::json::parse(data.data(), next(data.data(), data.size())).get<vector<order>>());
		}
		catch (const nlohmann::detail::parse_error& ex)
		{
			cout << "JSON ORDERS ERROR: " << ex.what() << endl;
		}
	}

	if (page_ < pages_ && !empty_page)
	{
		return make_shared<task_load_orders>(region_idx_, page_ + 1);
	}
	if (static_cast<size_t>(region_idx_ + 1) < ctx().region_ids().size())
	{
		return make_shared<task_load_orders>(region_idx_ + 1, 1);
	}
	ctx().clear_orders();
	return make_shared<task_load_orders>(0, 1);
}
