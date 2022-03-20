#pragma once
#include "handles.h"
#include "type.h"
#include "system.h"
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
	using type_dict_t = unordered_map<long long /*type_id*/, universe::type>;
	using system_dict_t = unordered_map<long long /*system_id*/, universe::system>;
	using region_no_market_t = unordered_map<long long /*region_id*/, int>;

// data
private:
	curl_global				cg_;
	shared_ptr<curl_multi>	multi_handle_{nullptr};
	tasks_t					tasks_;

	pqxx::connection		db_;

	vector<long long>		region_ids_;
	type_dict_t				type_dict_;
	system_dict_t			system_dict_;
	anomaly_sensor			anomaly_sensor_;
	region_no_market_t		region_no_market_;	// count of times when region had no market

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

	void set_type(universe::type&& t);
	const universe::type& type_by_id(long long type_id) const noexcept;

	void set_system(universe::system&& t);
	const universe::system& system_by_id(long long system_id) const noexcept;

	void add_region_ids(const vector<long long>& ids);
	const vector<long long>& region_ids() const noexcept { return region_ids_; }
	void set_region_has_market(long long region_id, bool has);
	bool region_has_market(long long region_id) const noexcept;

	void apply_orders(long long region_id, vector<order>&& orders);
	void clear_orders() noexcept { anomaly_sensor_.clear(); }
};

//---------------------------------------------------------------------------------------------------------
context& ctx();