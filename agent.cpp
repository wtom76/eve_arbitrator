#include "pch.h"
#include "agent.h"

universe::agent::agent(string name, string system, long long system_id)
	: name_{move(name)}
	, system_{move(system)}
	, system_id_{system_id}
{
}
