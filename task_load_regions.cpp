#include "pch.h"
#include "task_load_regions.h"
#include "task_load_orders.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_regions::task_load_regions(int page)
	: task{page}
{
	stringstream url;
	url << ctx().esi_subdir() << "/universe/regions/?datasource=" << ctx().esi_datasource();
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.str().c_str());

	//const string url{ctx().esi_subdir() + "/universe/regions/?datasource=" + ctx().esi_datasource()};
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_regions::_apply_raw_data(vector<char>&& data)
{
	cout << "task_load_regions done" << endl;

	try
	{
		ctx().add_region_ids(to_ints(data));
	}
	catch (const nlohmann::detail::exception& ex)
	{
		{
			ofstream f{"dumps/regions.dump", ios::binary|ios::trunc};
			f.write(data.data(), data.size());
		}
		cout << "JSON REGIONS ERROR: " << ex.what() << endl;
	}

	if (!ctx().region_ids().empty())
	{
		return make_shared<task_load_orders>(0, 1);
	}
	return {};
}
