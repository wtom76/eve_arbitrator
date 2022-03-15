#include <utility>
#include <cassert>
#include "task.h"

//---------------------------------------------------------------------------------------------------------
task::task()
{
	curl_easy_setopt(eh_.handle(), CURLOPT_WRITEFUNCTION, exec);
	curl_easy_setopt(eh_.handle(), CURLOPT_WRITEDATA, static_cast<void*>(this));

//	curl_easy_setopt(eh_.handle(), CURLOPT_VERBOSE, 1);
}
//---------------------------------------------------------------------------------------------------------
task::~task()
{
	deactivate();
}
//---------------------------------------------------------------------------------------------------------
void task::activate(shared_ptr<curl_multi> multi_handle)
{
	assert(!multi_handle_);
	assert(raw_data_.empty());

	multi_handle_ = move(multi_handle);
	curl_multi_add_handle(multi_handle_->handle(), eh_.handle());
}
//---------------------------------------------------------------------------------------------------------
void task::deactivate()
{
	if (multi_handle_)
	{
		curl_multi_remove_handle(multi_handle_->handle(), eh_.handle());
		multi_handle_.reset();
	}
}
//---------------------------------------------------------------------------------------------------------
void task::_exec(char* data, size_t n, size_t l)
{
	const size_t prev_size{raw_data_.size()};
	raw_data_.resize(prev_size + n * l);
	copy(data, data + n * l, raw_data_.data() + prev_size);
}
//---------------------------------------------------------------------------------------------------------
size_t task::exec(char* data, size_t n, size_t l, void* userp)
{
	static_cast<task*>(userp)->_exec(data, n, l);
	return n * l;
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task::finish()
{
	return _apply_raw_data(move(raw_data_));
}
