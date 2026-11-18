#include "LoadBalancer/Heuristics/CSharpHeuristic.h"

#include "Utility/Utils.h"

#ifdef __WITH_DOTNET_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/DotNetRuntime.h"

namespace framework::load_balancer
{
	CSharpHeuristic::CSharpHeuristic(std::string_view ip, std::string_view port, std::string_view heuristicName, const utility::LoadSource& source) :
		ip(ip),
		port(port),
		implementation(nullptr)
	{
		runtime::DotNetRuntime::NativeString moduleName = runtime::DotNetRuntime::getModuleName(std::get<std::filesystem::path>(source));
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		implementation = runtime.getCreateHeuristic()(std::format("{}, {}", heuristicName, moduleName.string()).data(), ip.data(), port.data());

		if (!implementation)
		{
			utility::logAndThrowException<logging::message::cantFindCSharpHeuristicSource, logging::category::csharpHeuristic>(std::any_cast<char*>(source));
		}
	}

	uint64_t CSharpHeuristic::operator ()() const
	{
		return runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().getCallHeuristic()(implementation);
	}

	void CSharpHeuristic::onStart()
	{
		runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().getOnStartHeuristic()(implementation);
	}

	void CSharpHeuristic::onEnd()
	{
		runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().getOnEndHeuristic()(implementation);
	}

	const std::string& CSharpHeuristic::getIp() const
	{
		return ip;
	}

	const std::string& CSharpHeuristic::getPort() const
	{
		return port;
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
