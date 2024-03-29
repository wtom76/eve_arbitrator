#include "pch.h"
#include "type.h"
#include "context.h"

namespace universe
{
	//---------------------------------------------------------------------------------------------------------
	void from_json(const nlohmann::json& j, type& dst)
	{
		const string error{read_optional<string>(j, "error"s)};
		if (!error.empty())
		{
			cout << error << endl;
		}
		dst.group_id_			= read_optional(j, "group_id"s, dst.group_id_);
		dst.market_group_id_	= read_optional(j, "market_group_id"s, dst.market_group_id_);
		dst.type_id_			= read_optional(j, "type_id"s, dst.type_id_);
		dst.packaged_volume_	= read_optional(j, "packaged_volume"s, dst.packaged_volume_);
		dst.name_				= read_optional(j, "name"s, dst.name_);
	}
}