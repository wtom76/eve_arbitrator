#include "pch.h"
#include "task_load_type.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_type::task_load_type(long long type_id)
	: task{0}
	, id_{type_id}
{
	const string url{ctx().esi_subdir() + "/universe/types/" + to_string(id_) + "?datasource=" + ctx().esi_datasource() + "&language=en"};
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_type::_apply_raw_data(vector<char>&& data)
{
	try
	{
		ctx().set_type(
			nlohmann::json::parse(data.data(), next(data.data(), data.size())).get<universe::type>()
		);
	}
	catch (const nlohmann::detail::exception& ex)
	{
		{
			ofstream f{"dumps/types/"s + to_string(id_) + ".dump", ios::binary|ios::trunc}; // trunc - hoping for single page
			f.write(data.data(), data.size());
		}
		cout << "JSON TYPE ERROR. type_id: " << id_ << "\n" << ex.what() << endl;
	}

	return {};
}
