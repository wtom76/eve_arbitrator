#include "pch.h"
#include "anomaly_sensor.h"
#include "context.h"
#include "task_load_type.h"
#include "task_load_system.h"

//---------------------------------------------------------------------------------------------------------
void anomaly_sensor::clear()
{
	item_map_.clear();
}
//---------------------------------------------------------------------------------------------------------
// TODO: consider location availability, qty
void anomaly_sensor::apply_orders(const vector<order>& orders)
{
	for (const order& ord : orders)
	{
		//_load_info(ord);

		item_market& market{item_map_[ord.type_id_]};
		if (ord.sell_)
		{
			if (!market.best_ask_.type_id_ || market.best_ask_.price_ > ord.price_)
			{
				market.best_ask_ = ord;
				if (market.best_bid_.type_id_)
				{
					_check_profit(market);
				}
			}
		}
		else
		{
			if (!market.best_bid_.type_id_ || market.best_bid_.price_ < ord.price_)
			{
				market.best_bid_ = ord;
				if (market.best_ask_.type_id_)
				{
					_check_profit(market);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------
bool anomaly_sensor::_load_info(const order& ord) const
{
	bool avaiable{true};
	if (!ctx().type_by_id(ord.type_id_).valid())
	{
		avaiable = false;
		ctx().add_task(make_shared<task_load_type>(ord.type_id_));
	}
	if (!ctx().system_by_id(ord.system_id_).valid())
	{
		avaiable = false;
		ctx().add_task(make_shared<task_load_system>(ord.system_id_));
	}
	return avaiable;
}
//---------------------------------------------------------------------------------------------------------
void anomaly_sensor::_check_profit(const item_market& market) const
{
	{
		const bool bid_ready{_load_info(market.best_bid_)};
		const bool ask_ready{_load_info(market.best_ask_)};
		if (!bid_ready || !ask_ready)
		{
			return;
		}
	}

	constexpr double profit_min_rate_{0.10};
	constexpr long long profit_min_val_{10000000};

	assert(market.best_ask_.type_id_);
	assert(market.best_ask_.type_id_ == market.best_bid_.type_id_);

	if (!market.best_bid_.price_ || !market.best_ask_.price_)
	{
		return;
	}

	const long long profit{market.best_bid_.price_ - market.best_ask_.price_};

	const universe::system& ask_system{ctx().system_by_id(market.best_ask_.system_id_)};
	const universe::system& bid_system{ctx().system_by_id(market.best_bid_.system_id_)};

	if (profit >= profit_min_val_ &&
		(static_cast<double>(profit) / market.best_bid_.price_) >= profit_min_rate_ &&
		bid_system.security_status_ >= 0.5)
	{
		cout << "PROFIT " << profit / 1000000 << "M\t"
			 << ctx().type_by_id(market.best_bid_.type_id_).name_ << "\t"
			 << ask_system.name_ << " (" << setprecision(2) << ask_system.security_status_ << ")\t"
			 << "ask: " << market.best_ask_.price_ << "\t"
			 << bid_system.name_ << " (" << setprecision(2) << bid_system.security_status_ << ")\t"
			 << "bid: " << market.best_bid_.price_ << "\t"
			 << "qty: " << min(market.best_bid_.volume_remain_, market.best_ask_.volume_remain_)
			 << endl;
	}
}
