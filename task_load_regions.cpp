#include "pch.h"
#include "task_load_regions.h"
#include "task_load_types.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_regions::task_load_regions(int page)
	: task{page}
{
	const string url{ctx().esi_subdir() + "/universe/regions/?datasource=" + ctx().esi_datasource()};
	cout << url << endl;
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_regions::_apply_raw_data(vector<char>&& data)
{
	//{
	//	ofstream f{"dumps/regions.dump", ios::binary|ios::trunc};
	//	f.write(data.data(), data.size());
	//}

	cout << "task_load_regions done" << endl;

	ctx().add_region_ids(to_ints(data));

	if (!ctx().region_ids().empty())
	{
		return make_shared<task_load_types>(1);
	}
	return {};
}
