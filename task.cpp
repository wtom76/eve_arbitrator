#include "pch.h"
#include "task.h"

//---------------------------------------------------------------------------------------------------------
task::task(int page)
	: page_{page}
{
	curl_easy_setopt(eh_.handle(), CURLOPT_HEADERDATA, static_cast<void*>(this));
	curl_easy_setopt(eh_.handle(), CURLOPT_HEADERFUNCTION, header_callback);

	curl_easy_setopt(eh_.handle(), CURLOPT_WRITEDATA, static_cast<void*>(this));
	curl_easy_setopt(eh_.handle(), CURLOPT_WRITEFUNCTION, data_callback);

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
void task::_header_callback(const char* data, size_t n, size_t /*always 1*/)
{
	const char* const data_end{next(data, n)};
	const auto key_end{find(data, data_end, ':')};
	const string key{trimed_lower_key({data, key_end})};
	if (key == "x-pages" && key_end != data_end)
	{
		pages_ = stoi(string{next(key_end), data_end});
	}
}
//---------------------------------------------------------------------------------------------------------
void task::_data_callback(const char* data, size_t n, size_t /*always 1*/)
{
	raw_data_.insert(raw_data_.end(), data, data + n);
}
//---------------------------------------------------------------------------------------------------------
bool task::_is_page_empty(const vector<char>& data) const noexcept
{
	return data.empty() || (data.size() >= 2 && (data[1] == ']' || data[1] == '}'));
}
//---------------------------------------------------------------------------------------------------------
size_t task::header_callback(char* data, size_t l, size_t n, void* userp)
{
	assert(sizeof(char) == l);
	static_cast<task*>(userp)->_header_callback(data, n, l);
	return n * l;
}
//---------------------------------------------------------------------------------------------------------
size_t task::data_callback(char* data, size_t l, size_t n, void* userp)
{
	assert(sizeof(char) == l);
	static_cast<task*>(userp)->_data_callback(data, n, l);
	return n * l;
}
//---------------------------------------------------------------------------------------------------------
shared_ptr<task> task::finish()
{
	return _apply_raw_data(move(raw_data_));
}
