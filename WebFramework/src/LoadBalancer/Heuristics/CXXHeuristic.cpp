#include "LoadBalancer/Heuristics/CXXHeuristic.h"

#include "Utility/Sources.h"
#include "Utility/Utils.h"

namespace framework::load_balancer
{
	CXXHeuristic::CXXHeuristic(std::string_view ip, std::string_view port, std::string_view heuristicName, HMODULE handle)
	{
		CreateHeuristicFunction creator = utility::load<CreateHeuristicFunction>(handle, std::format("create{}CXXHeuristic", heuristicName));

		if (!creator)
		{
			utility::logAndThrowException<logging::message::cantFindCXXHeuristic, logging::category::cxxHeuristic>(heuristicName);
		}

		implementation = creator(ip.data(), port.data());

		if (!implementation)
		{
			utility::logAndThrowException<logging::message::cantCreateCXXHeuristic, logging::category::cxxHeuristic>(heuristicName);
		}

		operatorFunction = utility::load<OperatorSignature>(handle, "webFrameworkCXXHeuristicOperator", true);
		onStartFunction = utility::load<OnStartSignature>(handle, "webFrameworkCXXHeuristicOnStart", true);
		onEndFunction = utility::load<OnEndSignature>(handle, "webFrameworkCXXHeuristicOnEnd", true);
		getIpFunction = utility::load<GetIpSignature>(handle, "webFrameworkCXXHeuristicGetIp", true);
		getPortFunction = utility::load<GetPortSignature>(handle, "webFrameworkCXXHeuristicGetPort", true);
		deleteHeuristicFunction = utility::load<DeleteHeuristicSignature>(handle, "webFrameworkCXXDeleteHeuristic", true);

		this->ip = getIpFunction(implementation);
		this->port = getPortFunction(implementation);
	}

	uint64_t CXXHeuristic::operator ()() const
	{
		return operatorFunction(implementation);
	}

	void CXXHeuristic::onStart()
	{
		return onStartFunction(implementation);
	}

	void CXXHeuristic::onEnd()
	{
		return onEndFunction(implementation);
	}

	const std::string& CXXHeuristic::getIp() const
	{
		return ip;
	}

	const std::string& CXXHeuristic::getPort() const
	{
		return port;
	}

	CXXHeuristic::~CXXHeuristic()
	{
		deleteHeuristicFunction(implementation);
	}
}
