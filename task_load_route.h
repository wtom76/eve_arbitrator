#pragma once
#include "task.h"
#include "route.h"

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task_load_route
	: public task
{
// data
private:
	universe::route route_{};

// methods
private:
	shared_ptr<task> _apply_raw_data(vector<char>&& data) override;

public:
	explicit task_load_route(const universe::route::key_t& key);
};