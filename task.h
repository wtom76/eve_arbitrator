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
	int						pages_{1};

// methods
private:
	void _header_callback(const char* data, size_t n, size_t l /*always 1*/);
	void _data_callback(const char* data, size_t n, size_t l /*always 1*/);

	virtual shared_ptr<task> _apply_raw_data(vector<char>&& data) = 0;

public:
	task();
	virtual ~task();
	void activate(shared_ptr<curl_multi> multi_handle);
	void deactivate();
	CURL* handle() const noexcept { return eh_.handle(); }

	static size_t header_callback(char* data, size_t l, size_t n, void* userp);
	static size_t data_callback(char* data, size_t l, size_t n, void* userp);

	shared_ptr<task> finish();
};