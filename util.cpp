#include "pch.h"
#include "util.h"

//---------------------------------------------------------------------------------------------------------
/// 1. find number position
vector<long long> to_ints(const vector<char>& data)
{
	vector<long long> result;

	if (data.size() <= 1)
	{
		return result;
	}

	const char* const data_end{data.data() + data.size()};
	const char* num_begin{data.data()};
	// 1.
	while (num_begin != data_end && !isdigit(*num_begin))
	{
		++num_begin;
	}
	const char* num_end{num_begin};

	while (num_begin != data_end)
	{
		while (num_end != data_end && isdigit(*num_end))
		{
			++num_end;
		}
		long long val{0};
		const from_chars_result res{from_chars(num_begin, num_end, val)};
		if (res.ec != std::errc{})
		{
			cout << "failed to read region id" << endl;
			return result;
		}
		// 1.
		num_begin = res.ptr;
		while (num_begin != data_end && !isdigit(*num_begin))
		{
			++num_begin;
		}

		result.emplace_back(val);
	}

	return result;
}
//---------------------------------------------------------------------------------------------------------
string trimed_lower_key(string s)
{
	static const string g_key_white_space(" \t");
	{
		const size_t pos{s.find_first_not_of(g_key_white_space)};
		if (pos != string::npos)
		{
			s.erase(0, pos);
		}
	}
	{
		const size_t pos{s.find_last_not_of(g_key_white_space)};
		if (pos != string::npos && pos + 1 != s.size())
		{
			s.erase(pos + 1);
		}
	}
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
	return s;
}
