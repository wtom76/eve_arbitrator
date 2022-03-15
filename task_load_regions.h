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
	void _finish() override;

	static vector<int> _to_ints(const vector<char>& data);

public:
	task_load_regions();
};