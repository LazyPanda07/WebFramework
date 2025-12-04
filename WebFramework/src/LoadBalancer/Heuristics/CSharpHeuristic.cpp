#include "CSharpHeuristic.h"

#include <Log.h>

#ifdef __WITH_DOT_NET_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/DotNetRuntime.h"

namespace framework::load_balancer
{
	CSharpHeuristic::CSharpHeuristic(std::string_view ip, std::string_view port, bool useHTTPS, std::string_view heuristicName, const utility::LoadSource& source) :
		ip(ip),
		port(port),
		useHTTPS(useHTTPS),
		implementation(nullptr)
	{
		char* assemblyName = std::any_cast<char*>(source);

		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		implementation = runtime.createHeuristic(assemblyName, ip.data(), port.data(), useHTTPS);

		if (!implementation)
		{
			if (Log::isValid())
			{
				Log::fatalError("Can't find {}", "LogCSharpHeuristic", 5, std::any_cast<char*>(source));
			}

			throw std::runtime_error(std::format("Can't find {}", std::any_cast<char*>(source)));
		}

		runtime.dealloc(assemblyName);
	}

	uint64_t CSharpHeuristic::operator ()() const
	{
		return runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().callHeuristic(implementation);
	}

	void CSharpHeuristic::onStart()
	{
		runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().onStartHeuristic(implementation);
	}

	void CSharpHeuristic::onEnd()
	{
		runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().onEndHeuristic(implementation);
	}

	const std::string& CSharpHeuristic::getIp() const
	{
		return ip;
	}

	const std::string& CSharpHeuristic::getPort() const
	{
		return port;
	}

	bool CSharpHeuristic::getUseHTTPS() const
	{
		return useHTTPS;
	}

	CSharpHeuristic::~CSharpHeuristic()
	{
		if (!implementation)
		{
			return;
		}

		runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().free(implementation);
	}
}

#endif
