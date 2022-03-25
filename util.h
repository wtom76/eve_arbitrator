#pragma once
#include "json.hpp"

//---------------------------------------------------------------------------------------------------------
enum class side
{
	null	= 0,
	bid		= 1,	// buy, bid
	ask		= 2		// sell, ask, offer
};

//---------------------------------------------------------------------------------------------------------
template <side s>
bool better(long long val, long long other) noexcept
{
	assert(false);
	return false;
}
template <>
inline bool better<side::bid>(long long val, long long other) noexcept
{
	return val > other;
}
template <>
inline bool better<side::ask>(long long val, long long other) noexcept
{
	return val < other;
}

//---------------------------------------------------------------------------------------------------------
vector<long long> to_ints(const vector<char>& data);

//---------------------------------------------------------------------------------------------------------
template <typename T>
inline T read_optional(const nlohmann::json& j, const std::string& field, const T& default_value = T{})
{
	const auto j_i = j.find(field);
	if (j_i != j.cend() && !j_i.value().is_null())
	{
		return j_i->get<T>();
	}
	return default_value;
}
//---------------------------------------------------------------------------------------------------------
template <typename T>
inline T read_optional(const nlohmann::json& j, const std::string& field, const T& default_value, bool& is_set)
{
	const auto j_i = j.find(field);
	if (j_i != j.cend() && !j_i.value().is_null())
	{
		is_set = true;
		return j_i->get<T>();
	}
	is_set = false;
	return default_value;
}
//---------------------------------------------------------------------------------------------------------
string trimmed_lower_key(string s);
//---------------------------------------------------------------------------------------------------------
string trimmed(string s);
