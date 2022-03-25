#pragma once
#include "handles.h"

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task
{
// data
protected:
	curl_easy				eh_;
	shared_ptr<curl_multi>	multi_handle_{nullptr};
	vector<char>			raw_data_;
	int						pages_{numeric_limits<int>::max()};
	int						page_{1};
	string					received_etag_;
	struct curl_slist*		http_request_headers_{nullptr};

// methods
private:
	void _header_callback(const char* data, size_t n, size_t l /*always 1*/);
	void _data_callback(const char* data, size_t n, size_t l /*always 1*/);

	virtual shared_ptr<task> _apply_raw_data(vector<char>&& data) = 0;

protected:
	long _http_response_code();
	bool _is_page_empty(const vector<char>& data) const noexcept;

public:
	task(int page);
	virtual ~task();
	void activate(shared_ptr<curl_multi> multi_handle);
	void deactivate();
	CURL* handle() const noexcept { return eh_.handle(); }

	static size_t header_callback(char* data, size_t l, size_t n, void* userp);
	static size_t data_callback(char* data, size_t l, size_t n, void* userp);

	shared_ptr<task> finish();
};