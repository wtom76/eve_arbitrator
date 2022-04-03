#include "pch.h"
#include "db.h"
#include "type.h"
#include "system.h"
#include "route.h"

//---------------------------------------------------------------------------------------------------------
db::db()
	: c_{"postgresql://eve:eve_pas@localhost:5432/eve"}
{
	c_.prepare("store_type",
		"INSERT INTO type VALUES ($1, $2, $3, $4, $5) ON CONFLICT (id) DO NOTHING;");

	c_.prepare("load_type",
		"SELECT group_id, market_group_id, packaged_volume, name FROM type WHERE id = $1");

	c_.prepare("store_system",
		"INSERT INTO system VALUES ($1, $2, $3, $4) ON CONFLICT (id) DO NOTHING;");

	c_.prepare("load_system",
		"SELECT security_status, constellation_id, name FROM system WHERE id = $1");

	c_.prepare("store_route",
		"INSERT INTO route VALUES ($1, $2, $3) ON CONFLICT DO NOTHING;");

	c_.prepare("load_route",
		"SELECT distance FROM route WHERE from_system_id = $1 AND to_system_id = $2");
}
//---------------------------------------------------------------------------------------------------------
void db::store(const universe::type& v)
{
	try
	{
		pqxx::work t{c_};
		t.exec_prepared("store_type",
			v.type_id_,
			v.group_id_,
			v.market_group_id_,
			v.packaged_volume_,
			v.name_);
		t.commit();
	}
	catch (const std::exception& ex)
	{
		cout << "Failed to store type. " << ex.what() << endl;
	}
}
//---------------------------------------------------------------------------------------------------------
bool db::load(universe::type& v)
{
	assert(v.type_id_ > 0);
	try
	{
		pqxx::work t{c_};
		pqxx::result r{t.exec_prepared("load_type", v.type_id_)};
		t.commit();
		if (r.empty())
		{
			v.type_id_ = 0;
			return false;
		}
		v.group_id_			= r.front()[0].as<decltype(v.group_id_)>();
		v.market_group_id_	= r.front()[1].as<decltype(v.market_group_id_)>();
		v.packaged_volume_	= r.front()[2].as<decltype(v.packaged_volume_)>();
		v.name_				= r.front()[3].as<decltype(v.name_)>();
	}
	catch (const std::exception& ex)
	{
		cout << "Failed to load type. " << ex.what() << endl;
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------
void db::store(const universe::system& v)
{
	try
	{
		pqxx::work t{c_};
		t.exec_prepared("store_system",
			v.system_id_,
			v.name_,
			v.security_status_,
			v.constellation_id_);
		t.commit();
	}
	catch (const std::exception& ex)
	{
		cout << "Failed to store system. " << ex.what() << endl;
	}
}
//---------------------------------------------------------------------------------------------------------
bool db::load(universe::system& v)
{
	assert(v.system_id_ > 0);
	try
	{
		pqxx::work t{c_};
		pqxx::result r{t.exec_prepared("load_system", v.system_id_)};
		t.commit();
		if (r.empty())
		{
			v.system_id_ = 0;
			return false;
		}
		v.security_status_	= r.front()[0].as<decltype(v.security_status_)>();
		v.constellation_id_	= r.front()[1].as<decltype(v.constellation_id_)>();
		v.name_				= r.front()[2].as<decltype(v.name_)>();
	}
	catch (const std::exception& ex)
	{
		cout << "Failed to load system. " << ex.what() << endl;
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------
void db::store(const universe::route& v)
{
	try
	{
		pqxx::work t{c_};
		t.exec_prepared("store_route",
			v.system_ids_.first,
			v.system_ids_.second,
			v.jumps_num_);
		t.commit();
	}
	catch (const std::exception& ex)
	{
		cout << "Failed to store route. " << ex.what() << endl;
	}
}
//---------------------------------------------------------------------------------------------------------
bool db::load(universe::route& v)
{
	assert(v.from_system_id() > 0);
	assert(v.to_system_id() > 0);
	try
	{
		pqxx::work t{c_};
		pqxx::result r{t.exec_prepared("load_route", v.from_system_id(), v.to_system_id())};
		t.commit();
		if (r.empty())
		{
			v.jumps_num_ = 0;
			return false;
		}
		v.jumps_num_ = r.front()[0].as<decltype(v.jumps_num_)>();
	}
	catch (const std::exception& ex)
	{
		cout << "Failed to load route. " << ex.what() << endl;
	}
	return true;
}
