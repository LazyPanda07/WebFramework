#pragma once

#include <LoadBalancerHeuristic.hpp>

class CustomHeuristic : public framework::LoadBalancerHeuristic
{
private:
	uint64_t id;

public:
	CustomHeuristic(std::string_view ip, std::string_view port);

	uint64_t operator ()() const override;

	~CustomHeuristic() = default;
};
