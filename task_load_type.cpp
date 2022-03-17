#include "pch.h"
#include "task_load_type.h"
#include "task_load_orders.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_type::task_load_type(long long type_idx, int page)
	: task{page}
	, type_idx_{type_idx}
{
	const string url{ctx().esi_subdir() + "/universe/types/" + to_string(type_id()) + "?datasource=" + ctx().esi_datasource()};
	cout << "task_load_type url: " << url << endl;
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());
}
//---------------------------------------------------------------------------------------------------------
long long task_load_type::type_id() const noexcept
{
	return ctx().type_ids()[type_idx_];
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_type::_apply_raw_data(vector<char>&& data)
{
	{
		ofstream f{"dumps/types/type_"s + to_string(type_id()) + ".dump", ios::binary|ios::trunc}; // trunc - hoping for single page
		f.write(data.data(), data.size());
	}

	from_json(nlohmann::json::parse(data.data(), next(data.data(), data.size())), type_);

	if (static_cast<size_t>(type_idx_ + 1) < ctx().type_ids().size())
	{
		ctx().set_type(move(type_));
		return make_shared<task_load_type>(type_idx_ + 1, 1);
	}
	if (!ctx().region_ids().empty())
	{
		ctx().set_type(move(type_));
		return make_shared<task_load_orders>(0, 1);
	}
	return {};
}
