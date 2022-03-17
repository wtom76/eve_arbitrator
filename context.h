#pragma once
#include "handles.h"
#include "type.h"
#include "order.h"
#include "anomaly_sensor.h"

class task;

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class context
{
// types
private:
	using tasks_t = unordered_map<CURL*, shared_ptr<task>>;
public:
	using type_dict_t = unordered_map<long long /*type_id*/, type>;

// data
private:
	curl_global				cg_;
	shared_ptr<curl_multi>	multi_handle_{nullptr};
	tasks_t					tasks_;
	vector<long long>		region_ids_;
	vector<long long>		type_ids_;
	type_dict_t				type_dict_;
	anomaly_sensor			anomaly_sensor_;

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

	void add_type_ids(const vector<long long>& ids);
	const vector<long long>& type_ids() const noexcept { return type_ids_; }
	void set_type(type&& t);
	const type& type_by_id(long long type_id) const noexcept;

	void add_region_ids(const vector<long long>& ids);
	const vector<long long>& region_ids() const noexcept { return region_ids_; }

	void apply_orders(long long region_id, vector<order>&& orders);
};

//---------------------------------------------------------------------------------------------------------
context& ctx();