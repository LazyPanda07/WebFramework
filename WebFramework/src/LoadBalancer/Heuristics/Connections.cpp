#include "Connections.h"

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		Connections::Connections(string_view ip, string_view port, bool useHTTPS) :
			BaseLoadBalancerHeuristic(ip, port, useHTTPS)
		{

		}

		void Connections::onStart()
		{
			connectons++;
		}

		void Connections::onEnd()
		{
			connectons--;
		}

		uint64_t Connections::operator ()() const
		{
			return connectons;
		}
	}
}
