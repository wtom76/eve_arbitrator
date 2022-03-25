#include "pch.h"
#include "anomaly_sensor.h"
#include "context.h"
#include "task_load_type.h"
#include "task_load_system.h"

//---------------------------------------------------------------------------------------------------------
void anomaly_sensor::clear()
{
	item_map_.clear();
	system_book_checked_ = false;
}
//---------------------------------------------------------------------------------------------------------
// TODO: consider location availability, qty
void anomaly_sensor::apply_orders(long long region_id, const vector<order>& orders)
{
	constexpr double min_ask_security{0.5};
	constexpr double min_bid_security{0.5};

	int num_info_requested{0};

	if (!system_book_checked_ && region_id > 10000002) // The Forge is passed
	{
		system_book_checked_ = true;
		//_check_system_books();
	}

	for (const order& ord : orders)
	{
		item_market& market{item_map_[ord.type_id_]};
		if (ord.sell_)
		{
			if (_load_info(ord, num_info_requested))
			{
				if (!market.best_ask_.type_id_ || market.best_ask_.price_ > ord.price_)
				{
					if (ctx().system_by_id(ord.system_id_).security_status_ >= min_ask_security)
					{
						market.best_ask_ = ord;
						if (market.best_bid_.type_id_)
						{
							_check_best_prices(market);
						}
					}
				}
				if (ctx().system_by_id(ord.system_id_).constellation_id_ == market.system_market_.constellation_id_)
				{
					market.system_market_.ask_book_.emplace(ord.price_, ord);
				}
			}
		}
		else
		{
			if (_load_info(ord, num_info_requested))
			{
				if (!market.best_bid_.type_id_ || market.best_bid_.price_ < ord.price_)
				{
					if (ctx().system_by_id(ord.system_id_).security_status_ >= min_bid_security)
					{
						market.best_bid_ = ord;
						if (market.best_ask_.type_id_)
						{
							_check_best_prices(market);
						}
					}
				}
				if (ctx().system_by_id(ord.system_id_).constellation_id_ == market.system_market_.constellation_id_)
				{
					market.system_market_.bid_book_.emplace(ord.price_, ord);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
bool anomaly_sensor::_load_info(const order& ord, int& num_info_requested) const
{
	bool avaiable{true};
	if (!ctx().type_by_id(ord.type_id_).valid())
	{
		avaiable = false;
		if (num_info_requested < 50)
		{
			++num_info_requested;
			ctx().add_task(make_shared<task_load_type>(ord.type_id_));
		}
	}
	if (!ctx().system_by_id(ord.system_id_).valid())
	{
		avaiable = false;
		if (num_info_requested < 50)
		{
			++num_info_requested;
			ctx().add_task(make_shared<task_load_system>(ord.system_id_));
		}
	}
	return avaiable;
}
//---------------------------------------------------------------------------------------------------------
void anomaly_sensor::_check_best_prices(const item_market& market) const
{
	constexpr double profit_min_rate{0.05};
	constexpr double sales_tax{0.036};
	constexpr long long profit_min_val{10000000};

	assert(market.best_ask_.type_id_);
	assert(market.best_ask_.type_id_ == market.best_bid_.type_id_);

	if (!market.best_bid_.price_ || !market.best_ask_.price_)
	{
		return;
	}

	const long long profit{market.best_bid_.price_ - market.best_ask_.price_ -
		static_cast<long long>(static_cast<double>(market.best_bid_.price_) * sales_tax)};

	const universe::system& ask_system{ctx().system_by_id(market.best_ask_.system_id_)};
	const universe::system& bid_system{ctx().system_by_id(market.best_bid_.system_id_)};

	if (profit >= profit_min_val && (static_cast<double>(profit) / market.best_bid_.price_) >= profit_min_rate)
	{
		cout << "HAUL " << profit / 1000000 << "M\t"
			 << ctx().type_by_id(market.best_bid_.type_id_).name_ << "\t"
			 << ask_system.name_ << " (" << setprecision(2) << ask_system.security_status_ << ")\t"
			 << "ask: " << market.best_ask_.price_ << "\t"
			 << bid_system.name_ << " (" << setprecision(2) << bid_system.security_status_ << ")\t"
			 << "bid: " << market.best_bid_.price_ << "\t"
			 << "qty: " << min(market.best_bid_.volume_remain_, market.best_ask_.volume_remain_)
			 << endl;
	}
}
//---------------------------------------------------------------------------------------------------------
void anomaly_sensor::_check_system_books() const
{
	for (const auto market_pr : item_map_)
	{
		_check_system_book(market_pr.first, market_pr.second);
	}
}
//---------------------------------------------------------------------------------------------------------
void anomaly_sensor::_check_system_book(long long type_id, const item_market& market) const
{
	static const unordered_set<long long> ingnored_goups
	{
		132,
		133,	// Energy Weapon Blueprint
		136,
		151,	// Energy Neutralizer Blueprint
		156,
		166,	// Missile Blueprint
		167,	// Hybrid Charge Blueprint
		168,
		343,
		349,
		360,
		787,	// Rig Blueprint
		912,
		965,	// Hybrid Component Blueprints
		1218,	// Resource Processing
		1271,	// Prosthetics
		1083,	// Eyewear
		1088,	// Outer
		1089,	// Tops
		1090,	// Bottoms
		1091,	// Footwear
		1194,	// Special Edition Commodities
		1670,	// Augmentations
		1888,
		1950	// Permanent SKIN
	};

	constexpr double profit_min_rate_{1.};
	constexpr long long profit_min_val_{1000000};

	if (market.system_market_.ask_book_.size() < 2)
	{
		return;
	}

	auto iter{market.system_market_.ask_book_.cbegin()};
	const long long best{iter->first};
	const long long next{(++iter)->first};
	if (!best || !next)
	{
		return;
	}
	if (ingnored_goups.find(ctx().type_by_id(iter->second.type_id_).group_id_) != ingnored_goups.cend())
	{
		return;
	}

	const long long profit{next - best};
	const double profit_rate{static_cast<double>(profit) / best};

	if (profit >= profit_min_val_ && profit_rate >= profit_min_rate_)
	{
		cout << "JITA " << profit / 1000000 << "M (" << setprecision(2) << profit_rate << ")\t"
			 << ctx().type_by_id(type_id).name_ << " (" << type_id << " < " << ctx().type_by_id(type_id).group_id_ << ")\t"
			 << "best ask: " << best << "\t"
			 << "next ask: " << next << "\t"
			 << endl;
	}
}
