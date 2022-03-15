#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "handles.h"
#include "order.h"

using namespace std;

class task;

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class context
{
// types
private:
	using tasks_t = unordered_map<CURL*, shared_ptr<task>>;

// data
private:
	curl_global				cg_;
	shared_ptr<curl_multi>	multi_handle_{nullptr};
	tasks_t					tasks_;
	vector<long long>		region_ids_;

	void _run();

public:
	context();

	void start();
	void add_task(shared_ptr<task> task);

	void set_region_ids(vector<long long>&& ids) noexcept { region_ids_ = move(ids); }
	const vector<long long>& region_ids() const noexcept { return region_ids_; }

	void apply_orders(long long region_id, vector<order>&& orders);
};

//---------------------------------------------------------------------------------------------------------
context& ctx();