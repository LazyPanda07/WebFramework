#pragma once

#include "BaseLoadBalancerHeuristic.h"

namespace framework
{
	namespace load_balancer
	{
		class ConnectionsHeuristic : public BaseLoadBalancerHeuristic
		{
		public:
			ConnectionsHeuristic(std::string_view ip, std::string_view port, bool useHTTPS);

			uint64_t operator ()() const override;

			~ConnectionsHeuristic() = default;
		};
	}
}
