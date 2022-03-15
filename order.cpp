#include <vector>
#include "order.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------
void from_json(const nlohmann::json& j, order& dst)
{
	dst.sell_			= j.at("is_buy_order").get<decltype(dst.sell_)>();
	dst.location_id_	= j.at("location_id").get<decltype(dst.location_id_)>();
	dst.min_volume_		= j.at("min_volume").get<decltype(dst.min_volume_)>();
	dst.order_id_		= j.at("order_id").get<decltype(dst.order_id_)>();
	dst.price_			= j.at("price").get<decltype(dst.price_)>();
	dst.system_id_		= j.at("system_id").get<decltype(dst.system_id_)>();
	dst.type_id_		= j.at("type_id").get<decltype(dst.type_id_)>();
	dst.volume_remain_	= j.at("volume_remain").get<decltype(dst.volume_remain_)>();
	dst.volume_total_	= j.at("volume_total").get<decltype(dst.volume_total_)>();
}
//---------------------------------------------------------------------------------------------------------
// TODO:
//std::istream& operator >>(std::istream& s, vector<order>& cfg)
