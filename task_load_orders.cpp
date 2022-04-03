#include "pch.h"
#include "task_load_orders.h"
#include "context.h"
#include "order.h"

//---------------------------------------------------------------------------------------------------------
task_load_orders::task_load_orders(long long region_idx, int page)
	: task{page}
	, region_idx_{region_idx}
{
	assert(region_idx_ >= 0);
	assert(static_cast<size_t>(region_idx_) < ctx().region_ids().size());

	stringstream url;
	url << ctx().esi_subdir() << "/markets/" << _region_id() << "/orders/?datasource=" << ctx().esi_datasource()
		<< "&order_type=all&page=" << page_;
	curl_easy_setopt(eh_.handle(), CURLOPT_URL, url.str().c_str());

	//cout << url.str() << endl;
	//const string url{ctx().esi_subdir() + "/markets/" + to_string(_region_id()) + "/orders/?datasource=" + ctx().esi_datasource() + "&order_type=all&page=" + to_string(page_)};

	{
		const string last_etag{ctx().region_orders_etag(_region_id(), page_)};
		if (!last_etag.empty())
		{
			_fill_etag_header(last_etag);
			//cout << etag_header_ << endl;
			http_request_headers_ = curl_slist_append(http_request_headers_, etag_header_);
			curl_easy_setopt(eh_.handle(), CURLOPT_HTTPHEADER, http_request_headers_);
		}
	}

	{
		ofstream f{"dumps/orders.dump", ios::binary|ios::trunc};
		f.close();
	}
}
//---------------------------------------------------------------------------------------------------------
void task_load_orders::_fill_etag_header(const string& etag)
{
	static const char etag_key[]{"If-None-Match: "};

	if (etag.size() + sizeof(etag_key) > sizeof(etag_header_))
	{
		assert(false);
		return;
	}
	memcpy(etag_header_, "If-None-Match: ", sizeof(etag_key) - 1);
	memcpy(etag_header_ + sizeof(etag_key) - 1, etag.c_str(), etag.size() + 1);
}
//---------------------------------------------------------------------------------------------------------
long long task_load_orders::_region_id(long long region_idx) const noexcept
{
	return ctx().region_ids()[region_idx ? region_idx : region_idx_];
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_orders::_apply_raw_data(vector<char>&& data)
{
	//{
	//	ofstream f{"dumps/orders.dump", ios::binary|ios::app};
	//	f.write(data.data(), data.size());
	//	f.close();
	//}

	switch (_http_response_code())
	{
	case 0:
		return {};
	case 304:
		if (ctx().apply_cached_orders(_region_id(), page_) > 0 && page_ < pages_)
		{
			// same region next page
			return make_shared<task_load_orders>(region_idx_, page_ + 1);
		}
		break;
	case 200:
		ctx().set_region_orders_etag(_region_id(), page_, received_etag_);
		if (_is_page_empty(data))
		{
			ctx().set_region_has_market(_region_id(), false);
		}
		else
		{
			ctx().set_region_has_market(_region_id(), true);
			try
			{
				ctx().apply_orders(_region_id(), page_, nlohmann::json::parse(data.data(), next(data.data(), data.size())).get<vector<order>>());
			}
			catch (const nlohmann::detail::exception& ex)
			{
				cout << "JSON ORDERS ERROR. _region_id: " << _region_id() << "\n" << ex.what() << endl;
			}
			// same region next page
			if (page_ < pages_)
			{
				return make_shared<task_load_orders>(region_idx_, page_ + 1);
			}
		}
		break;
	}
	return _next_region();
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task_load_orders::_next_region()
{
	// next region
	long long new_region_idx{region_idx_ + 1};
	while (static_cast<size_t>(new_region_idx) < ctx().region_ids().size()
		&& !ctx().region_has_market(_region_id(new_region_idx))
		)
	{
		++new_region_idx;
	}
	if (static_cast<size_t>(new_region_idx) < ctx().region_ids().size())
	{
		return make_shared<task_load_orders>(new_region_idx, 1);
	}

	// start from first region
	ctx().clear_orders();

	new_region_idx = 0;
	while (static_cast<size_t>(new_region_idx) < ctx().region_ids().size()
		&& !ctx().region_has_market(_region_id(new_region_idx))
		)
	{
		++new_region_idx;
	}
	if (static_cast<size_t>(new_region_idx) < ctx().region_ids().size())
	{
		return make_shared<task_load_orders>(new_region_idx, 1);
	}
	return {};
}
