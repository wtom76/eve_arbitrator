#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include "task_load_orders.h"
#include "context.h"
#include "order.h"

//---------------------------------------------------------------------------------------------------------
task_load_orders::task_load_orders(long long region_idx)
{
	assert(region_idx >= 0);
	assert(static_cast<size_t>(region_idx) < ctx().region_ids().size());

	region_id_ = ctx().region_ids()[region_idx];

	const string url{"https://esi.evetech.net/dev/markets/"s + to_string(region_id_) + "/orders/?datasource=tranquility&order_type=all"};
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());

	cout << "task_load_orders started with url" << endl;
}
//---------------------------------------------------------------------------------------------------------
void task_load_orders::_apply_raw_data(vector<char>&& data)
{
	cout << "task_load_orders done" << endl;

	fstream f{"orders.dump", ios::out|ios::binary|ios::trunc};
	f.write(data.data(), data.size());

	ctx().apply_orders(region_id_, nlohmann::json::parse(data.data(), next(data.data(), data.size())).get<vector<order>>());

	if (static_cast<size_t>(region_id_ + 1) < ctx().region_ids().size())
	{
		ctx().add_task(make_shared<task_load_orders>(region_id_ + 1));
	}
}
