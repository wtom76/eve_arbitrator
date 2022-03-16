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

	void _clear();
	void _run();

public:
	context();

	/// ESI URL starting part up to subdir. example: https://esi.evetech.net/dev
	const string& esi_subdir() const noexcept;
	/// value for datasource field in URL. example: tranquility
	const string& esi_datasource() const noexcept;

	void start();
	void add_task(shared_ptr<task> task);

	void add_region_ids(vector<long long>&& ids);
	const vector<long long>& region_ids() const noexcept { return region_ids_; }

	void apply_orders(long long region_id, vector<order>&& orders);
};

//---------------------------------------------------------------------------------------------------------
context& ctx();