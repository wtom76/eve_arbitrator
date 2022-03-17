#pragma once
#include "task.h"

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task_load_regions
	: public task
{
// data
private:
// methods
private:
	shared_ptr<task> _apply_raw_data(vector<char>&& data) override;

public:
	explicit task_load_regions(int page);
};