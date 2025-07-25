#pragma once

#include <LoadBalancerHeuristics.hpp>

class CustomHeuristic : public framework::BaseLoadBalancerHeuristic
{
private:
	uint64_t id;

public:
	CustomHeuristic(std::string_view ip, std::string_view port, bool useHTTPS);

	uint64_t operator ()() const override;

	~CustomHeuristic() = default;
};
