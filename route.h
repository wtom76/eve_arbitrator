#pragma once
#include "json.hpp"

namespace universe
{
	//---------------------------------------------------------------------------------------------------------
	/// struct route
	//---------------------------------------------------------------------------------------------------------
	struct route
	{
		using key_t = pair<long long, long long>;

		key_t		system_ids_{0, 0};
		long long	jumps_num_{0};

		route() = default;
		// will sort system_id's
		route(key_t key);

		static void norm(key_t& key) noexcept;
		long long from_system_id() const noexcept { return system_ids_.first; }
		long long to_system_id() const noexcept { return system_ids_.second; }
		bool zero() const noexcept { return from_system_id() == to_system_id(); }
		bool valid() const noexcept { return from_system_id() != 0; }
		// read from ESI response raw data
		void read(const vector<char>& data);
	};
}

//---------------------------------------------------------------------------------------------------------
// id, id pair hash
namespace std
{
	template<>
	struct hash<universe::route::key_t>
	{
		constexpr size_t operator()(const universe::route::key_t& key) const noexcept
		{
			return (key.first << (4/*bytes*/ * 8/*bits*/)) | key.second;
		}
	};
}