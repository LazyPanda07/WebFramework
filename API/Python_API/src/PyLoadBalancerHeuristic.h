#pragma once

#include <LoadBalancerHeuristic.hpp>

namespace framework
{
	class PyLoadBalancerHeuristic : public LoadBalancerHeuristic
	{
	public:
		PyLoadBalancerHeuristic(std::string_view ip, std::string_view port, bool useHTTPS);

		uint64_t operator ()() const override;

		void onStart() override;

		void onEnd() override;

		~PyLoadBalancerHeuristic() = default;
	};
}
