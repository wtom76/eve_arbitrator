#include "pch.h"
#include "task_load_type.h"
#include "task_load_orders.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_type::task_load_type(long long type_id)
	: task{0}
	, type_id_{type_id}
{
	const string url{ctx().esi_subdir() + "/universe/types/" + to_string(type_id) + "?datasource=" + ctx().esi_datasource()};
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_type::_apply_raw_data(vector<char>&& data)
{
	{
		ofstream f{"dumps/types/"s + to_string(type_id_) + ".dump", ios::binary|ios::trunc}; // trunc - hoping for single page
		f.write(data.data(), data.size());
	}

	try
	{
		ctx().set_type(
			nlohmann::json::parse(data.data(), next(data.data(), data.size())).get<type>()
		);
	}
	catch (const nlohmann::detail::parse_error& ex)
	{
		cout << "JSON TYPE ERROR: " << ex.what() << endl;
	}

	return {};
}
