#include "pch.h"
#include "task_load_system.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_system::task_load_system(long long system_id)
	: task{0}
	, id_{system_id}
{
	const string url{ctx().esi_subdir() + "/universe/systems/" + to_string(id_) + "?datasource=" + ctx().esi_datasource() + "&language=en"};
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_system::_apply_raw_data(vector<char>&& data)
{
	//{
	//	ofstream f{"dumps/systems/"s + to_string(id_) + ".dump", ios::binary|ios::trunc}; // trunc - hoping for single page
	//	f.write(data.data(), data.size());
	//}

	try
	{
		ctx().set_system(
			nlohmann::json::parse(data.data(), next(data.data(), data.size())).get<universe::system>()
		);
	}
	catch (const nlohmann::detail::parse_error& ex)
	{
		cout << "JSON SYSTEM ERROR: " << ex.what() << endl;
	}

	return {};
}
