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

		const string& BaseLoadBalancerHeuristic::getIp() const
		{
			return ip;
		}

		const string& BaseLoadBalancerHeuristic::getPort() const
		{
			return port;
		}

		bool BaseLoadBalancerHeuristic::getUseHTTPS() const
		{
			return useHTTPS;
		}
	}
}
