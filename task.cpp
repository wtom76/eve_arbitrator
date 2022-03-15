#include <utility>
#include <cassert>
#include "task.h"

//---------------------------------------------------------------------------------------------------------
task::task()
{
	curl_easy_setopt(eh_.handle(), CURLOPT_WRITEFUNCTION, exec);
	curl_easy_setopt(eh_.handle(), CURLOPT_WRITEDATA, static_cast<void*>(this));
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
void task::exec(char* data, size_t n, size_t l, void* userp)
{
	const size_t prev_size{raw_data_.size()};
	raw_data_.resize(raw_data_.size() + n * l);
	copy(data, data + n * l, raw_data_.data());
}
//---------------------------------------------------------------------------------------------------------
void task::finish()
{
	_finish();
}
