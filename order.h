#pragma once
#include "json.hpp"

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
struct order
{
	bool sell_{false};
	long long location_id_{0};
	long long min_volume_{0};
	long long order_id_{0};
	long long price_{0};
	long long system_id_{0};
	long long type_id_{0};
	long long volume_remain_{0};
	long long volume_total_{0};
};

//---------------------------------------------------------------------------------------------------------
void from_json(const nlohmann::json& j, order& dst);
