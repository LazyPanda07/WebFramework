#pragma once

#include "core.h"

namespace framework
{
	namespace load_balancer
	{
		class BaseLoadBalancerHeuristic
		{
		public:
			BaseLoadBalancerHeuristic() = default;

			virtual ~BaseLoadBalancerHeuristic() = default;
		};
	}
}
