#include "pch.h"
#include "task_load_types.h"
#include "task_load_type.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_types::task_load_types(int page)
	: task{page}
{
	const string url{ctx().esi_subdir() + "/universe/types/?datasource=" + ctx().esi_datasource() + "&page=" + to_string(page_)};
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.c_str());

	ofstream f{"dumps/types.dump", ios::binary|ios::trunc};
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_types::_apply_raw_data(vector<char>&& data)
{
	{
		ofstream f{"dumps/types.dump", ios::binary|ios::app};
		f.write(data.data(), data.size());
	}

	const bool empty_page{_is_page_empty(data)};
	cout << "task_load_types page " << page_ << " done. empty: " << empty_page << endl;

	if (!empty_page)
	{
		ctx().add_type_ids(to_ints(data));
	}
	if (page_ < pages_ && !empty_page)
	{
		return make_shared<task_load_types>(page_ + 1);
	}
	if (!ctx().type_ids().empty())
	{
		return make_shared<task_load_type>(0, 1);
	}
	return {};
}
