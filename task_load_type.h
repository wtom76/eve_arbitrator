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
	long long	type_id_{0};

// methods
private:
	long long type_id() const noexcept;
	shared_ptr<task> _apply_raw_data(vector<char>&& data) override;

public:
	/// region_idx is an index in context::region_ids() vector. not region_id itself.
	explicit task_load_type(long long type_id);
};