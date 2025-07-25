#include "CXXHeuristic.h"

using namespace std;

namespace framework::load_balancer
{
	CXXHeuristic::CXXHeuristic(void* implementation, string_view ip, string_view port, bool useHTTPS) :
		BaseLoadBalancerHeuristic(ip, port, useHTTPS),
		implementation(implementation)
	{

	}

	uint64_t CXXHeuristic::operator ()() const
	{
		return 0;
	}

	void CXXHeuristic::onStart()
	{

	}

	void CXXHeuristic::onEnd()
	{

	}

	const string& CXXHeuristic::getIp() const
	{
		static string test;

		return test;
	}

	const string& CXXHeuristic::getPort() const
	{
		static string test;

		return test;
	}

	bool CXXHeuristic::getUseHTTPS() const
	{
		return false;
	}

	CXXHeuristic::~CXXHeuristic()
	{

	}
}
