#include "pch.h"
#include "db.h"
#include "type.h"

//---------------------------------------------------------------------------------------------------------
db::db()
	: c_{"postgresql://eve:eve_pas@localhost:5432/eve"}
{
	c_.prepare("store_type",
		"INSERT INTO type VALUES ($1, $2, $3, $4, $5) ON CONFLICT (id) DO NOTHING;");

	c_.prepare("load_type",
		"SELECT group_id, market_group_id, volume, name FROM type WHERE id = $1");
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
			v.volume_,
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
		v.volume_			= r.front()[2].as<decltype(v.volume_)>();
		v.name_				= r.front()[3].as<decltype(v.name_)>();
	}
	catch (const std::exception& ex)
	{
		cout << "Failed to load type. " << ex.what() << endl;
	}
	return true;
}
