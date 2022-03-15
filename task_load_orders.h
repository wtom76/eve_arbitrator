#pragma once
#include "task.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task_load_orders
	: public task
{
// data
// methods
private:
	void _finish() override;

public:
	task_load_orders();
};