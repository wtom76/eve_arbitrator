#include "pch.h"
#include "task_load_regions.h"
#include "task_load_orders.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_regions::task_load_regions(int page)
	: task{page}
{
	const string url{ctx().esi_subdir() + "/universe/regions/?datasource=" + ctx().esi_datasource()};
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_regions::_apply_raw_data(vector<char>&& data)
{
	{
		ofstream f{"dumps/regions.dump", ios::binary|ios::trunc};
		f.write(data.data(), data.size());
	}

	cout << "task_load_regions done" << endl;

	try
	{
		ctx().add_region_ids(to_ints(data));
	}
	catch (const nlohmann::detail::parse_error& ex)
	{
		cout << "JSON REGIONS ERROR: " << ex.what() << endl;
	}

	if (!ctx().region_ids().empty())
	{
		return make_shared<task_load_orders>(0, 1);
	}
	return {};
}
