#include "Connections.h"

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		Connections::Connections(string_view ip, string_view port, bool useHTTPS) :
			BaseLoadBalancerHeuristic(ip, port, useHTTPS),
			connections(0)
		{

		}

		void Connections::onStart()
		{
			connections++;
		}

		void Connections::onEnd()
		{
			connections--;
		}

		uint64_t Connections::operator ()() const
		{
			return connections;
		}
	}
}
