#include "BaseLoadBalancerHeuristic.h"

using namespace std;

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

		void BaseLoadBalancerHeuristic::onStart()
		{

		}

		void BaseLoadBalancerHeuristic::onEnd()
		{

		}
	}
}
