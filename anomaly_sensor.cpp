#include "pch.h"
#include "anomaly_sensor.h"
#include "context.h"
#include "task_load_type.h"

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
		_load_type(ord.type_id_);

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
void anomaly_sensor::_load_type(long long type_id) const
{
	if (!ctx().type_by_id(type_id).valid())
	{
		ctx().add_task(make_shared<task_load_type>(type_id));
	}
}
//---------------------------------------------------------------------------------------------------------
void anomaly_sensor::_check_profit(const item_market& market) const
{
	constexpr double profit_min_rate_{0.05};
	constexpr long long profit_min_val_{1000000};

	assert(market.best_ask_.type_id_);
	assert(market.best_ask_.type_id_ == market.best_bid_.type_id_);

	if (!market.best_bid_.price_ || !market.best_ask_.price_)
	{
		return;
	}

	const long long profit{market.best_bid_.price_ - market.best_ask_.price_};

	if (profit >= profit_min_val_ && (static_cast<double>(profit) / market.best_bid_.price_) >= profit_min_rate_)
	{
		cout << "FOUND PROFIT. type_id: " << market.best_bid_.type_id_
			 << " bid: " << market.best_bid_.price_
			 << " ask: " << market.best_ask_.price_
			 << " profit: " << profit
			 << "\t" << ctx().type_by_id(market.best_bid_.type_id_).name_
			 << endl;
	}
}
