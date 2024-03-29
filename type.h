#pragma once
#include "json.hpp"

namespace universe
{
	//---------------------------------------------------------------------------------------------------------
	/// struct type
	//---------------------------------------------------------------------------------------------------------
	struct type
	{
		long long	type_id_{0};
		long long	group_id_{0};
		long long	market_group_id_{0};
		double		packaged_volume_{0.};
		string		name_;

		bool valid() const noexcept { return type_id_ > 0; }
	};

	//---------------------------------------------------------------------------------------------------------
	void from_json(const nlohmann::json& j, type& dst);
}