#pragma once
#include "task.h"

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task_load_regions
	: public task
{
// data
private:
	int page_{1};
// methods
private:
	shared_ptr<task> _apply_raw_data(vector<char>&& data) override;

	static vector<long long> _to_ints(const vector<char>& data);

public:
	explicit task_load_regions(int page);
};