#pragma once

namespace universe
{
	//---------------------------------------------------------------------------------------------------------
	// struct agent
	//---------------------------------------------------------------------------------------------------------
	struct agent
	{
		string		name_;
		string		system_;
		long long	system_id_{0};

		agent() = default;
		agent(string name, string system, long long system_id);

		bool valid() const noexcept { return system_id_ > 0; }
	};
}
