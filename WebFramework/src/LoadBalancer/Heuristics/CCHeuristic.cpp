#include "CCHeuristic.h"

#include "Utility/Sources.h"

#define ASSERT_LOAD_FUNCTION(name) if (!static_cast<bool>(name)) throw runtime_error(format("Can't load {} function", #name))

using namespace std;

namespace framework::load_balancer
{
	CCHeuristic::CCHeuristic(string_view ip, string_view port, bool useHTTPS, string_view heuristicName, HMODULE handle)
	{
		CreateHeuristicFunction creator = utility::load<CreateHeuristicFunction>(handle, format("create{}CCHeuristic", heuristicName));

		if (!creator)
		{
			throw runtime_error(format("Can't find {} function", heuristicName));
		}

		implementation = creator(ip.data(), port.data(), useHTTPS);

		if (!implementation)
		{
			throw runtime_error(format("Can't create {} heuristic", heuristicName));
		}

		operatorFunction = utility::load<OperatorSignature>(handle, format("webFrameworkCCHeuristicOperator{}", heuristicName));
		initFunction = utility::load<InitSignature>(handle, format("webFrameworkCCHeuristicInit{}", heuristicName));
		onStartFunction = utility::load<OnStartSignature>(handle, format("webFrameworkCCHeuristicOnStart{}", heuristicName));
		onEndFunction = utility::load<OnEndSignature>(handle, format("webFrameworkCCHeuristicOnEnd{}", heuristicName));
		getIpFunction = utility::load<GetIpSignature>(handle, format("webFrameworkCCHeuristicGetIp{}", heuristicName));
		getPortFunction = utility::load<GetPortSignature>(handle, format("webFrameworkCCHeuristicGetPort{}", heuristicName));
		getUseHTTPSFunction = utility::load<GetUseHTTPSSignature>(handle, format("webFrameworkCCHeuristicGetUseHTTPS{}", heuristicName));
		deleteHeuristicFunction = utility::load<DeleteHeuristicSignature>(handle, format("webFrameworkCCDeleteHeuristic{}", heuristicName));

		ASSERT_LOAD_FUNCTION(operatorFunction);
		ASSERT_LOAD_FUNCTION(deleteHeuristicFunction);

		if (initFunction)
		{
			initFunction(this);
		}

		if (!onStartFunction)
		{
			onStartFunction = [](void*) {};
		}

		if (!onEndFunction)
		{
			onEndFunction = [](void*) {};
		}

		this->ip = getIpFunction(implementation);
		this->port = getPortFunction(implementation);
	}

	uint64_t CCHeuristic::operator ()() const
	{
		return operatorFunction(implementation);
	}

	void CCHeuristic::onStart()
	{
		return onStartFunction(implementation);
	}

	void CCHeuristic::onEnd()
	{
		return onEndFunction(implementation);
	}

	const string& CCHeuristic::getIp() const
	{
		return ip;
	}

	const string& CCHeuristic::getPort() const
	{
		return port;
	}

	bool CCHeuristic::getUseHTTPS() const
	{
		return getUseHTTPSFunction(implementation);
	}

	CCHeuristic::~CCHeuristic()
	{
		deleteHeuristicFunction(implementation);
	}
}
