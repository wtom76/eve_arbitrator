#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include "task_load_orders.h"
#include "context.h"
#include "order.h"

//---------------------------------------------------------------------------------------------------------
task_load_orders::task_load_orders(long long region_idx)
	: region_idx_{region_idx}
{
	assert(region_idx_ >= 0);
	assert(static_cast<size_t>(region_idx_) < ctx().region_ids().size());

	const string url{"https://esi.evetech.net/dev/markets/"s + to_string(region_id()) + "/orders/?datasource=tranquility&order_type=all"};
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());

	cout << "task_load_orders started with url" << endl;
}
//---------------------------------------------------------------------------------------------------------
long long task_load_orders::region_id() const noexcept
{
	return ctx().region_ids()[region_idx_];
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_orders::_apply_raw_data(vector<char>&& data)
{
	cout << "task_load_orders done" << endl;

	{
		fstream f{"orders.dump", ios::out|ios::binary|ios::trunc};
		f.write(data.data(), data.size());
	}

	ctx().apply_orders(region_id(), nlohmann::json::parse(data.data(), next(data.data(), data.size())).get<vector<order>>());

	if (static_cast<size_t>(region_idx_ + 1) < ctx().region_ids().size())
	{
		return make_shared<task_load_orders>(region_idx_ + 1);
	}
	return {};
}
