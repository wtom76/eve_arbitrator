#pragma once
#include "task.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task_load_orders
	: public task
{
// data
private:
	long long region_id_{0};
// methods
private:
	void _apply_raw_data(vector<char>&& data) override;

public:
	/// region_idx is an index in context::region_ids() vector. not region_id itself.
	explicit task_load_orders(long long region_idx);
};