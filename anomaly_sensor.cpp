#include "pch.h"
#include "anomaly_sensor.h"
#include "context.h"

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
void anomaly_sensor::_check_profit(const item_market& market) const
{
	assert(market.best_ask_.type_id_);
	assert(market.best_ask_.type_id_ == market.best_bid_.type_id_);

	if (market.best_ask_.price_ < market.best_bid_.price_)
	{
		cout << "FOUND PROFIT. type_id: " << market.best_bid_.type_id_
			 << " ask: " << market.best_ask_.price_
			 << " bid: " << market.best_bid_.price_
			 << "\t" << ctx().type_by_id(market.best_bid_.type_id_).name_
			 << endl;
	}
}
