#pragma once
#include "json.hpp"

namespace universe
{
	//---------------------------------------------------------------------------------------------------------
	/// struct system
	//---------------------------------------------------------------------------------------------------------
	struct system
	{
		long long	system_id_{0};
		long long	constellation_id_{0};
		double		security_status_{0.};
		string		name_;

		bool valid() const noexcept { return system_id_ > 0; }
	};

	//---------------------------------------------------------------------------------------------------------
	void from_json(const nlohmann::json& j, system& dst);
}
