#pragma once
#include "task.h"

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task_load_orders
	: public task
{
// data
private:
	long long	region_idx_{0};
	char		etag_header_[1024];

// methods
private:
	void _fill_etag_header(const string& etag);
	long long _region_id(long long region_idx = 0) const noexcept;
	shared_ptr<task> _apply_raw_data(vector<char>&& data) override;
	shared_ptr<task> _next_region();

public:
	/// region_idx is an index in context::region_ids() vector. not region_id itself.
	task_load_orders(long long region_idx, int page);
};