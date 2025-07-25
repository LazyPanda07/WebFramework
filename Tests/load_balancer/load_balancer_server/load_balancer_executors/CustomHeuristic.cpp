#include "CustomHeuristic.h"

static uint64_t getId();

CustomHeuristic::CustomHeuristic(std::string_view ip, std::string_view port, bool useHTTPS) :
	LoadBalancerHeuristic(ip, port, useHTTPS),
	id(getId())
{

}

uint64_t CustomHeuristic::operator ()() const
{
	return id;
}

uint64_t getId()
{
	static std::atomic_uint64_t id = 0;

	return id++;
}

DECLARE_HEURISTIC(CustomHeuristic)
