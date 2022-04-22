#include "pch.h"
#include "agent.h"

universe::agent::agent(string name, string account, string system, long long system_id)
	: name_{move(name)}
	, account_{move(account)}
	, system_{move(system)}
	, system_id_{system_id}
{
}
