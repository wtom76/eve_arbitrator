#pragma once
#include "handles.h"
#include "type.h"
#include "system.h"
#include "order.h"
#include "anomaly_sensor.h"

class task;
class db;

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
	using orders_by_region_t = unordered_map<long long /*region_id << 10 | page*/, vector<order>>;
	using region_order_etags_t = unordered_map<long long /*region_id << 10 | page*/, string>;

// data
private:
	curl_global				cg_;
	shared_ptr<curl_multi>	multi_handle_{nullptr};
	tasks_t					tasks_;

	unique_ptr<db>			db_;

	vector<long long>		region_ids_;
	type_dict_t				type_dict_;
	system_dict_t			system_dict_;
	anomaly_sensor			anomaly_sensor_;
	region_no_market_t		region_no_market_;	// count of times when region had no market
	orders_by_region_t		orders_by_region_;
	region_order_etags_t	region_order_etags_;

// methods
private:
	void _run();
	long long _region_page_hash(long long region_id, int page) const noexcept;

public:
	context();

	/// ESI URL starting part up to subdir. example: https://esi.evetech.net/dev
	const string& esi_subdir() const noexcept;
	/// value for datasource field in URL. example: tranquility
	const string& esi_datasource() const noexcept;

	void start();
	void add_task(shared_ptr<task> task);

	void set_type(universe::type&& t);
	const universe::type& type_by_id(long long type_id) noexcept;

	void set_system(universe::system&& t);
	const universe::system& system_by_id(long long system_id) const noexcept;

	void add_region_ids(const vector<long long>& ids);
	const vector<long long>& region_ids() const noexcept { return region_ids_; }
	void set_region_has_market(long long region_id, bool has);
	bool region_has_market(long long region_id) const noexcept;

	void clear_orders() noexcept { anomaly_sensor_.clear(); }

	void apply_orders(long long region_id, int page, vector<order>&& orders);
	size_t apply_cached_orders(long long region_id, int page);
	void set_region_orders_etag(long long region_id, int page, string etag) noexcept;
	string region_orders_etag(long long region_id, int page) noexcept;
};

//---------------------------------------------------------------------------------------------------------
context& ctx();