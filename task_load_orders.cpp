#include <iostream>
#include <algorithm>
#include <fstream>
#include <charconv>
#include <cctype>
#include "task_load_orders.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_orders::task_load_orders()
{
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, "https://esi.evetech.net/dev/universe/regions/?datasource=tranquility");
}
//---------------------------------------------------------------------------------------------------------
void task_load_orders::_finish()
{
	cout << "task_load_orders done" << endl;

	fstream f{"regions.dump", ios::out|ios::binary|ios::trunc};
	f.write(raw_data_.data(), raw_data_.size());
	ctx().set_region_ids(_to_ints(raw_data_));
	raw_data_.clear();
}
