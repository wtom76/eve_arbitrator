#pragma once
#include "task.h"
#include "type.h"

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task_load_type
	: public task
{
// data
private:
	long long id_{0};	// type_id

// methods
private:
	shared_ptr<task> _apply_raw_data(vector<char>&& data) override;

public:
	explicit task_load_type(long long type_id);
};