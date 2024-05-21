#include "ConnectionsHeuristic.h"

namespace framework
{
	namespace load_balancer
	{
		ConnectionsHeuristic::ConnectionsHeuristic(string_view ip, string_view port, bool useHTTPS) :
			BaseLoadBalancerHeuristic(ip, port, useHTTPS)
		{

		}

		uint64_t ConnectionsHeuristic::operator ()() const
		{
			return 0;
		}

		DECLARE_HEURISTIC(ConnectionsHeuristic)
	}
}
