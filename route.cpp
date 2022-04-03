#include "pch.h"
#include "route.h"
#include "context.h"

namespace universe
{
	//---------------------------------------------------------------------------------------------------------
	route::route(key_t key)
		: system_ids_{key}
	{
		assert(system_ids_.first > 0);
		assert(system_ids_.second > 0);

		norm(system_ids_);
		if (system_ids_.first == system_ids_.second)
		{
			jumps_num_ = 1;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void route::norm(key_t& key) noexcept
	{
		if (key.first > key.second)
		{
			swap(key.first, key.second);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void route::read(const vector<char>& data)
	{
		jumps_num_ = to_ints(data).size();
	}
}