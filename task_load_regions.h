#pragma once
#include <vector>
#include "task.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task_load_regions
	: public task
{
// data
// methods
private:
	void _apply_raw_data(vector<char>&& data) override;

	static vector<long long> _to_ints(const vector<char>& data);

public:
	task_load_regions();
};