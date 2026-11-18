#include "CustomHeuristic.h"

#include <atomic>

static uint64_t getId();

CustomHeuristic::CustomHeuristic(std::string_view ip, std::string_view port) :
	LoadBalancerHeuristic(ip, port),
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

DEFINE_HEURISTIC(CustomHeuristic)
