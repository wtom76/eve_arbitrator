#pragma once
#include "order.h"
#include "route.h"

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
	void _check_best_prices(const item_market& market) const;
	bool _load_info(const order& ord, int& num_info_requested) const;				// type, system, etc. currently will appear on next hit. returns true if info already available
	bool _load_route(universe::route::key_t key, int& num_info_requested) const;	// type, system, etc. currently will appear on next hit. returns true if info already available
public:
	void clear();
	void apply_orders(const vector<order>& orders);
};