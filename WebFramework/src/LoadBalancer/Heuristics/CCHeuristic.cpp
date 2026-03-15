#include "LoadBalancer/Heuristics/CCHeuristic.h"

#include "Utility/Sources.h"
#include "Utility/Utils.h"

namespace framework::load_balancer
{
	CCHeuristic::CCHeuristic(std::string_view ip, std::string_view port, bool useHTTPS, std::string_view heuristicName, HMODULE handle)
	{
		CreateHeuristicFunction creator = utility::load<CreateHeuristicFunction>(handle, std::format("create{}CCHeuristic", heuristicName));

		if (!creator)
		{
			utility::logAndThrowException<logging::message::cantFindCCHeuristic, logging::category::ccHeuristic>(heuristicName);
		}

		implementation = creator(ip.data(), port.data(), useHTTPS);

		if (!implementation)
		{
			utility::logAndThrowException<logging::message::cantCreateCCHeuristic, logging::category::ccHeuristic>(heuristicName);
		}

		initFunction = utility::load<InitSignature>(handle, std::format("webFrameworkCCHeuristicInit{}", heuristicName));
		onStartFunction = utility::load<OnStartSignature>(handle, std::format("webFrameworkCCHeuristicOnStart{}", heuristicName));
		onEndFunction = utility::load<OnEndSignature>(handle, std::format("webFrameworkCCHeuristicOnEnd{}", heuristicName));
		getIpFunction = utility::load<GetIpSignature>(handle, std::format("webFrameworkCCHeuristicGetIp{}", heuristicName));
		getPortFunction = utility::load<GetPortSignature>(handle, std::format("webFrameworkCCHeuristicGetPort{}", heuristicName));
		getUseHTTPSFunction = utility::load<GetUseHTTPSSignature>(handle, std::format("webFrameworkCCHeuristicGetUseHTTPS{}", heuristicName));
		operatorFunction = utility::load<OperatorSignature>(handle, std::format("webFrameworkCCHeuristicOperator{}", heuristicName), true);
		deleteHeuristicFunction = utility::load<DeleteHeuristicSignature>(handle, std::format("webFrameworkCCDeleteHeuristic{}", heuristicName), true);

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

	const std::string& CCHeuristic::getIp() const
	{
		return ip;
	}

	const std::string& CCHeuristic::getPort() const
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
