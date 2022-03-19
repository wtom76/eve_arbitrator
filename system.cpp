#include "pch.h"
#include "system.h"

namespace universe
{
	//---------------------------------------------------------------------------------------------------------
	void from_json(const nlohmann::json& j, system& dst)
	{
		const string error{read_optional<string>(j, "error"s)};
		if (!error.empty())
		{
			cout << error << endl;
		}
		dst.system_id_			= read_optional(j, "system_id"s, dst.system_id_);
		dst.constellation_id_	= read_optional(j, "constellation_id"s, dst.constellation_id_);
		dst.security_status_	= read_optional(j, "security_status"s, dst.security_status_);
		dst.name_				= read_optional(j, "name"s, dst.name_);
	}
}