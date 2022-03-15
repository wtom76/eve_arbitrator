#pragma once

#include <memory>
#include <curl/curl.h>

using namespace std;

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class curl_easy
{
	CURL* eh_{nullptr};
public:
	curl_easy();
	curl_easy(curl_easy&& src);

	curl_easy& operator = (curl_easy&& src) = delete;
	curl_easy(const curl_easy&) = delete;
	curl_easy& operator = (const curl_easy&) = delete;

	~curl_easy();

	CURL* handle() const noexcept { return eh_; }
};
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class curl_multi
{
	CURLM* multi_handle_{nullptr};
public:
	curl_multi();
	~curl_multi();

	curl_multi(curl_multi&& src) = delete;
	curl_multi(const curl_multi&) = delete;
	curl_multi& operator = (curl_multi&& src) = delete;
	curl_multi& operator = (const curl_multi&) = delete;

	CURLM* handle() const noexcept { return multi_handle_; }
};
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
class curl_global
{
public:
	curl_global();
	~curl_global();
};
