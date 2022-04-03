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
	struct system_item_market
	{
		template <side s>
		struct price_less
		{
			bool operator()(long long lhs, long long rhs) const noexcept
			{
				return better<s>(lhs, rhs);
			}
		};
		using bid_book_t = map<long long /*price*/, order, price_less<side::bid>>;
		using ask_book_t = map<long long /*price*/, order, price_less<side::ask>>;

		bid_book_t bid_book_;
		ask_book_t ask_book_;

		long long constellation_id_{20000020};	// 20000020 - Kimotoro
	};

	struct item_market
	{
		order best_bid_;
		order best_ask_;

		system_item_market system_market_;	// single system market
	};
	using item_map_t = unordered_map<long long /*type_id*/, item_market>;

// data
private:
	item_map_t item_map_;
	bool system_book_checked_{false};

// methods
private:
	void _check_best_prices(const item_market& market) const;
	void _check_system_books() const;
	void _check_system_book(long long type_id, const item_market& market) const;
	bool _load_info(const order& ord, int& num_info_requested) const;				// type, system, etc. currently will appear on next hit. returns true if info already available
	bool _load_route(universe::route::key_t key, int& num_info_requested) const;	// type, system, etc. currently will appear on next hit. returns true if info already available
public:
	void clear();
	void apply_orders(long long region_id, const vector<order>& orders);
};