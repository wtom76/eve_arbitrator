#pragma once
#include "order.h"

//---------------------------------------------------------------------------------------------------------
/// class anomaly_sensor
/// finds profitable opportinities from price deviations
//---------------------------------------------------------------------------------------------------------
class anomaly_sensor
{
// types
private:
	struct item_market
	{
		order best_bid_;
		order best_ask_;
	};
	using item_map_t = unordered_map<long long /*type_id*/, item_market>;

// data
private:
	item_map_t item_map_;

// methods
private:
	void _check_profit(const item_market& market) const;
	bool _load_info(const order& ord) const;				// type, system, etc. currently will appear on next hit. returns true if info already available
public:
	void clear();
	void apply_orders(const vector<order>& orders);
};