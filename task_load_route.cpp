#include "pch.h"
#include "task_load_route.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_route::task_load_route(const universe::route::key_t& key)
	: task{0}
	, route_{key}
{
	assert(!route_.zero());

	stringstream url;
	url << ctx().esi_subdir() << "/route/"
		<< route_.from_system_id() << '/' << route_.to_system_id()
		<< "/?datasource=" << ctx().esi_datasource() << "&flag=secure";
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.str().c_str());

#if defined PRINT_URLS
	cout << url.str() << endl;
#endif
	//const string url{ctx().esi_subdir() + "/route/" + to_string(route_.from_system_id_) + '/' + to_string(route_.to_system_id_) + "/?datasource=" + ctx().esi_datasource() + "&flag=secure"};
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_route::_apply_raw_data(vector<char>&& data)
{
	try
	{
		route_.read(data);
		ctx().set_route(route_);
	}
	catch (const nlohmann::detail::exception& ex)
	{
		{
			ofstream f{"dumps/routes/"s + to_string(route_.from_system_id()) + '_' + to_string(route_.to_system_id()) + ".dump", ios::binary|ios::trunc}; // trunc - hoping for single page
			f.write(data.data(), data.size());
		}
		cout << "JSON ROUTE ERROR. " << to_string(route_.from_system_id()) << '/' << to_string(route_.to_system_id()) << "\n" << ex.what() << endl;
	}

	return {};
}
