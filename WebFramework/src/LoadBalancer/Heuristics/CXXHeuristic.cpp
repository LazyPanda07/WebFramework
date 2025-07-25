#include "CXXHeuristic.h"

#include "Utility/Sources.h"

#define ASSERT_LOAD_FUNCTION(name) if (!static_cast<bool>(name)) throw runtime_error(format("Can't load {} function", #name))

using namespace std;

namespace framework::load_balancer
{
	CXXHeuristic::CXXHeuristic(string_view ip, string_view port, bool useHTTPS, string_view heuristicName, HMODULE handle)
	{
		CreateHeuristicFunction creator = utility::load<CreateHeuristicFunction>(handle, format("create{}Heuristic", heuristicName));

		if (!creator)
		{
			throw runtime_error(format("Can't find {} function", heuristicName));
		}

		implementation = creator(ip.data(), port.data(), useHTTPS);

		if (!implementation)
		{
			throw runtime_error(format("Can't create {} heuristic", heuristicName));
		}

		operatorFunction = utility::load<OperatorSignature>(handle, "webFrameworkHeuristicOperator");
		onStartFunction = utility::load<OnStartSignature>(handle, "webFrameworkHeuristicOnStart");
		onEndFunction = utility::load<OnEndSignature>(handle, "webFrameworkHeuristicOnEnd");
		getIpFunction = utility::load<GetIpSignature>(handle, "webFrameworkHeuristicGetIp");
		getPortFunction = utility::load<GetPortSignature>(handle, "webFrameworkHeuristicGetPort");
		getUseHTTPSFunction = utility::load<GetUseHTTPSSignature>(handle, "webFrameworkHeuristicGetUseHTTPS");
		deleteHeuristicFunction = utility::load<DeleteHeuristicSignature>(handle, "webFrameworkDeleteHeuristic");

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

	const string& CXXHeuristic::getIp() const
	{
		return ip;
	}

	const string& CXXHeuristic::getPort() const
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
