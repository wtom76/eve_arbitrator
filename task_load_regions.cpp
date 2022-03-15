#include <iostream>
#include <algorithm>
#include <fstream>
#include <charconv>
#include <cctype>
#include "task_load_regions.h"
#include "task_load_orders.h"
#include "context.h"

//---------------------------------------------------------------------------------------------------------
task_load_regions::task_load_regions()
{
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, "https://esi.evetech.net/dev/universe/regions/?datasource=tranquility");
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_regions::_apply_raw_data(vector<char>&& data)
{
	cout << "task_load_regions done" << endl;

	fstream f{"regions.dump", ios::out|ios::binary|ios::trunc};
	f.write(data.data(), data.size());
	ctx().set_region_ids(_to_ints(data));
	if (!ctx().region_ids().empty())
	{
		return make_shared<task_load_orders>(0);
	}
	return {};
}
//---------------------------------------------------------------------------------------------------------
/// 1. find number position
vector<long long> task_load_regions::_to_ints(const vector<char>& data)
{
	vector<long long> result;

	if (data.size() <= 1)
	{
		return result;
	}

	const char* const data_end{data.data() + data.size()};
	const char* num_begin{data.data()};
	// 1.
	while (num_begin != data_end && !isdigit(*num_begin))
	{
		++num_begin;
	}
	const char* num_end{num_begin};

	while (num_begin != data_end)
	{
		while (num_end != data_end && isdigit(*num_end))
		{
			++num_end;
		}
		long long val{0};
		const from_chars_result res{from_chars(num_begin, num_end, val)};
		if (res.ec != std::errc{})
		{
			cout << "failed to read region id" << endl;
			return result;
		}
		// 1.
		num_begin = res.ptr;
		while (num_begin != data_end && !isdigit(*num_begin))
		{
			++num_begin;
		}

		result.emplace_back(val);
		cout << "region id: " << val << endl;
	}

	return result;
}
