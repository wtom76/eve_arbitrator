#pragma once
#include <pqxx/pqxx>	// requires higher C++ version than currently installed

namespace universe
{
	struct type;
	struct system;
	struct route;
	struct agent;
}

//---------------------------------------------------------------------------------------------------------
/// class db
//---------------------------------------------------------------------------------------------------------
class db
{
private:
	pqxx::connection c_;

public:
	db();

	void store(const universe::type& v);
	bool load(universe::type& v);
	void store(const universe::system& v);
	bool load(universe::system& v);
	void store(const universe::route& v);
	bool load(universe::route& v);

	bool load(vector<universe::agent>& v);
};