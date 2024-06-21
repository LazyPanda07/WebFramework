#pragma once

#include "LoadBalancer/Heuristics/BaseLoadBalancerHeuristic.h"

class CustomHeuristic : public framework::load_balancer::BaseLoadBalancerHeuristic
{
private:
	uint64_t id;

public:
	CustomHeuristic(std::string_view ip, std::string_view port, bool useHTTPS);

	uint64_t operator ()() const override;

	~CustomHeuristic() = default;
};
