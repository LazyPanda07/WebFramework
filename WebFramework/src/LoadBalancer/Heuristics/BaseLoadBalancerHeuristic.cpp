#include "BaseLoadBalancerHeuristic.h"

namespace framework
{
	namespace load_balancer
	{
		BaseLoadBalancerHeuristic::BaseLoadBalancerHeuristic(string_view ip, string_view port, bool useHTTPS) :
			ip(ip),
			port(port),
			useHTTPS(useHTTPS)
		{

		}
	}
}
