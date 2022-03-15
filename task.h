#pragma once
#include <vector>
#include "handles.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class task
{
// data
protected:
	curl_easy				eh_;
	shared_ptr<curl_multi>	multi_handle_{nullptr};
	vector<char>			raw_data_;

// methods
private:
	void _exec(char* data, size_t n, size_t l);

	virtual void _apply_raw_data(vector<char>&& data) = 0;

public:
	task();
	virtual ~task();
	void activate(shared_ptr<curl_multi> multi_handle);
	void deactivate();
	CURL* handle() const noexcept { return eh_.handle(); }

	static void exec(char* data, size_t n, size_t l, void* userp);

	void finish();
};