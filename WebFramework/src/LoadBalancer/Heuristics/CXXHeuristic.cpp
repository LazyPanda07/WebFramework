#include "CXXHeuristic.h"

#include <Log.h>

#include "Utility/Sources.h"

#define ASSERT_LOAD_FUNCTION(name) if (!static_cast<bool>(name)) throw std::runtime_error(std::format("Can't load {} function", #name))

namespace framework::load_balancer
{
	CXXHeuristic::CXXHeuristic(std::string_view ip, std::string_view port, bool useHTTPS, std::string_view heuristicName, HMODULE handle)
	{
		CreateHeuristicFunction creator = utility::load<CreateHeuristicFunction>(handle, std::format("create{}CXXHeuristic", heuristicName));

		if (!creator)
		{
			if (Log::isValid())
			{
				Log::fatalError("Can't find create{}CXXHeuristic function", "LogCXXHeuristic", 3, heuristicName);
			}

			throw std::runtime_error(std::format("Can't find create{}CXXHeuristic function", heuristicName));
		}

		implementation = creator(ip.data(), port.data(), useHTTPS);

		if (!implementation)
		{
			if (Log::isValid())
			{
				Log::fatalError("Can't create create{}CXXHeuristic heuristic", "LogCXXHeuristic", 4, heuristicName);
			}

			throw std::runtime_error(std::format("Can't create create{}CXXHeuristic heuristic", heuristicName));
		}

		operatorFunction = utility::load<OperatorSignature>(handle, "webFrameworkCXXHeuristicOperator");
		onStartFunction = utility::load<OnStartSignature>(handle, "webFrameworkCXXHeuristicOnStart");
		onEndFunction = utility::load<OnEndSignature>(handle, "webFrameworkCXXHeuristicOnEnd");
		getIpFunction = utility::load<GetIpSignature>(handle, "webFrameworkCXXHeuristicGetIp");
		getPortFunction = utility::load<GetPortSignature>(handle, "webFrameworkCXXHeuristicGetPort");
		getUseHTTPSFunction = utility::load<GetUseHTTPSSignature>(handle, "webFrameworkCXXHeuristicGetUseHTTPS");
		deleteHeuristicFunction = utility::load<DeleteHeuristicSignature>(handle, "webFrameworkCXXDeleteHeuristic");

		ASSERT_LOAD_FUNCTION(operatorFunction);
		ASSERT_LOAD_FUNCTION(onStartFunction);
		ASSERT_LOAD_FUNCTION(onEndFunction);
		ASSERT_LOAD_FUNCTION(getIpFunction);
		ASSERT_LOAD_FUNCTION(getPortFunction);
		ASSERT_LOAD_FUNCTION(getUseHTTPSFunction);
		ASSERT_LOAD_FUNCTION(deleteHeuristicFunction);

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

	bool CXXHeuristic::getUseHTTPS() const
	{
		return getUseHTTPSFunction(implementation);
	}

	CXXHeuristic::~CXXHeuristic()
	{
		deleteHeuristicFunction(implementation);
	}
}
