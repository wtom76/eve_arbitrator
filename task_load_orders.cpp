#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include "task_load_orders.h"
#include "context.h"
#include "order.h"

//---------------------------------------------------------------------------------------------------------
task_load_orders::task_load_orders(long long region_idx, int page)
	: region_idx_{region_idx}
	, page_{page}
{
	assert(region_idx_ >= 0);
	assert(static_cast<size_t>(region_idx_) < ctx().region_ids().size());

	const string url{ctx().esi_subdir() + "/markets/" + to_string(region_id()) + "/orders/?datasource=" + ctx().esi_datasource() + "&order_type=all&page=" + to_string(page_)};
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());
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
		fstream f{"orders.dump", ios::out|ios::binary|ios::trunc};
		f.write(data.data(), data.size());
	}

	ctx().apply_orders(region_id(), nlohmann::json::parse(data.data(), next(data.data(), data.size())).get<vector<order>>());

	if (page_ < pages_)
	{
		return make_shared<task_load_orders>(region_idx_, page_ + 1);
	}
	if (static_cast<size_t>(region_idx_ + 1) < ctx().region_ids().size())
	{
		return make_shared<task_load_orders>(region_idx_ + 1, 1);
	}
	return {};
}
