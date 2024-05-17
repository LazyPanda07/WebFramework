#pragma once

#include "BaseLoadBalancerHeuristic.h"

namespace framework
{
	namespace load_balancer
	{
		class ConnectionsHeuristic : public BaseLoadBalancerHeuristic
		{
		public:
			ConnectionsHeuristic() = default;

			~ConnectionsHeuristic() = default;
		};
	}
}
