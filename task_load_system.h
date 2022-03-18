#pragma once
#include "task.h"
#include "system.h"

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task_load_system
	: public task
{
// data
private:
	long long id_{0};

// methods
private:
	shared_ptr<task> _apply_raw_data(vector<char>&& data) override;

public:
	explicit task_load_system(long long system_id);
};