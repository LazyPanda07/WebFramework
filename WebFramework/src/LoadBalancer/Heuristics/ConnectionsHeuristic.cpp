#include "ConnectionsHeuristic.h"

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		ConnectionsHeuristic::ConnectionsHeuristic(string_view ip, string_view port, bool useHTTPS) :
			BaseLoadBalancerHeuristic(ip, port, useHTTPS)
		{

		}

		void ConnectionsHeuristic::onStart()
		{
			connectons++;
		}

		void ConnectionsHeuristic::onEnd()
		{
			connectons--;
		}

		uint64_t ConnectionsHeuristic::operator ()() const
		{
			return connectons;
		}

		DECLARE_HEURISTIC(ConnectionsHeuristic)
	}
}
