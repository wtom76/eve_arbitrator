#include "pch.h"
#include "anomaly_sensor.h"
#include "context.h"
#include "agent.h"
#include "task_load_type.h"
#include "task_load_system.h"
#include "task_load_route.h"

//---------------------------------------------------------------------------------------------------------
void anomaly_sensor::clear()
{
	item_map_.clear();
}
//---------------------------------------------------------------------------------------------------------
// TODO: consider location availability, qty
void anomaly_sensor::apply_orders(const vector<order>& orders)
{
	constexpr double min_ask_security{-10.};
	constexpr double min_bid_security{0.5};

	int num_info_requested{0};

	for (const order& ord : orders)
	{
		if (!_load_info(ord, num_info_requested))
		{
			continue;
		}

		item_market& market{item_map_[ord.type_id_]};

		if (ord.sell_)
		{
			if ((!market.best_ask_.type_id_ || market.best_ask_.price_ > ord.price_)
				&& ctx().system_by_id(ord.system_id_).security_status_ >= min_ask_security)
			{
				market.best_ask_ = ord;
				if (market.best_bid_.type_id_ &&
					_load_route({market.best_ask_.system_id_, market.best_bid_.system_id_}, num_info_requested))
				{
					_check_best_prices(market);
				}
			}
		}
		else
		{
			if ((!market.best_bid_.type_id_ || market.best_bid_.price_ < ord.price_)
				&& ctx().system_by_id(ord.system_id_).security_status_ >= min_bid_security)
			{
				market.best_bid_ = ord;
				if (market.best_ask_.type_id_ &&
					_load_route({market.best_ask_.system_id_, market.best_bid_.system_id_}, num_info_requested))
				{
					_check_best_prices(market);
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
		if (num_info_requested < 30)
		{
			++num_info_requested;
			ctx().add_task(make_shared<task_load_type>(ord.type_id_));
		}
	}
	if (!ctx().system_by_id(ord.system_id_).valid())
	{
		avaiable = false;
		if (num_info_requested < 30)
		{
			++num_info_requested;
			ctx().add_task(make_shared<task_load_system>(ord.system_id_));
		}
	}
	return avaiable;
}
//---------------------------------------------------------------------------------------------------------
bool anomaly_sensor::_load_route(universe::route::key_t key, int& num_info_requested) const
{
	if (!ctx().route_by_id(key).valid())
	{
		if (num_info_requested < 30)
		{
			++num_info_requested;
			ctx().add_task(make_shared<task_load_route>(key));
		}
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------
static const char* txt_color(double ask_sec_status, double cargo_volume)
{
	if (cargo_volume <= 96.)
	{
		if (ask_sec_status < 0.5)
		{
			return "91";
		}
		else
		{
			return "92";
		}
	}
	else
	{
		if (ask_sec_status < 0.5)
		{
			return "31";
		}
		else
		{
			return "32";
		}
	}
}
//---------------------------------------------------------------------------------------------------------
void anomaly_sensor::_check_best_prices(const item_market& market) const
{
	constexpr double sales_tax{0.036};
	constexpr double profit_min_rate{0.05};
	constexpr double min_profit_per_jump{1000000.};
	constexpr long long profit_min_val{10000000};

	assert(market.best_ask_.type_id_);
	assert(market.best_ask_.type_id_ == market.best_bid_.type_id_);

	if (!market.best_bid_.price_ || !market.best_ask_.price_)
	{
		return;
	}

	const long long profit{(market.best_bid_.price_ - market.best_ask_.price_ -
		static_cast<long long>(static_cast<double>(market.best_bid_.price_) * sales_tax)) *
		std::min(market.best_bid_.volume_remain_, market.best_ask_.volume_remain_)
	};
	const universe::route& route{ctx().route_by_id({market.best_ask_.system_id_, market.best_bid_.system_id_})};

	if (
		profit >= profit_min_val &&
		(static_cast<double>(profit) / market.best_bid_.price_) >= profit_min_rate &&
		(!route.jumps_num_ || static_cast<double>(profit) / route.jumps_num_ >= min_profit_per_jump)
	)
	{
		const universe::system& ask_system{ctx().system_by_id(market.best_ask_.system_id_)};
		const universe::system& bid_system{ctx().system_by_id(market.best_bid_.system_id_)};
		universe::route agent_route;
		agent_route.system_ids_.second = market.best_ask_.system_id_;
		const universe::agent* agent{ctx().nearest_agent(agent_route)};

		const long long min_qty{ min(market.best_bid_.volume_remain_, market.best_ask_.volume_remain_) };
		const universe::type& type{ ctx().type_by_id(market.best_bid_.type_id_) };
		const double cargo_volume{ type.packaged_volume_ * min_qty };

		cout << "\033[" << txt_color(ask_system.security_status_, cargo_volume) << "m"
			 << "h" << setw(5) << profit / 1000000 << "m"
			 << "  <" << setw(2) << route.jumps_num_ - 1 << ">"
			 << " " << type.name_ << " x " << min_qty
			 << " [" << fixed << setprecision(1) << cargo_volume << " m3]"
			 << "\t" << ask_system.name_ << " (" << setprecision(2) << ask_system.security_status_ << ")"
			 << "\t" << bid_system.name_ << " (" << setprecision(2) << bid_system.security_status_ << ")"
			 << "\task: " << market.best_ask_.price_ << " x " << market.best_ask_.volume_remain_
			 << "\tbid: " << market.best_bid_.price_ << " x " << market.best_bid_.volume_remain_
			 << "\t{" << market.best_ask_.price_ * min_qty << " isk}"
			 << " " << (agent ? agent->name_ : ""s) << " '" << (agent ? agent->account_ : ""s) << "' <" << agent_route.jumps_num_ - 1 << ">"
			 << "\033[0m"
			 << endl;
	}
}
