#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "handles.h"

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
	vector<int>				region_ids_;

	void _run();

public:
	context();

	void start();
	void set_region_ids(vector<int>&& ids) noexcept { region_ids_ = move(ids); }
};

context& ctx();