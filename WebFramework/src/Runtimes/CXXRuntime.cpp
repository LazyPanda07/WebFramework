#include "Runtimes/CXXRuntime.h"

#include <Log.h>

#include "Executors/CXXExecutor.h"
#include "Utility/DynamicLibraries.h"
#include "Framework/WebFrameworkConstants.h"
#include "TaskBroker/TaskExecutors/CXXTaskExecutor.h"

namespace framework::runtime
{
	void CXXRuntime::finishInitialization()
	{

	}

	bool CXXRuntime::loadExecutor(std::string_view name, std::string_view route, const utility::LoadSource& source)
	{
		if (!std::holds_alternative<HMODULE>(source))
		{
			return false;
		}

		HMODULE module = std::get<HMODULE>(source);
		std::string creatorFunctionName = std::format("create{}CXXInstance", name);

		if (CreateExecutorSignature creator = utility::load<CreateExecutorSignature>(module, creatorFunctionName))
		{
			std::filesystem::path sourcePath = utility::getPathToLibrary(module);

			if (Log::isValid())
			{
				Log::info("Found {} in {} for {} route", "LogWebFrameworkInitialization", creatorFunctionName, sourcePath.empty() ? "current" : sourcePath.string(), route.empty() ? R"("")" : route);
			}

			creators.emplace(name, std::make_tuple(module, creator));

			return true;
		}

		return false;
	}

	std::unique_ptr<Executor> CXXRuntime::createExecutor(std::string_view name) const
	{
		auto it = creators.find(name);

		if (it == creators.end())
		{
			throw std::runtime_error(std::format("Can't find executor with name {}", name));
		}

		const auto& [_, creatorData] = *it;
		const auto& [module, creator] = creatorData;

		return std::make_unique<CXXExecutor>(module, creator());
	}

	std::unique_ptr<task_broker::TaskExecutor> CXXRuntime::createTaskExecutor(std::string_view name, const utility::LoadSource& source) const
	{
		if (!std::holds_alternative<HMODULE>(source))
		{
			return nullptr;
		}

		HMODULE module = std::get<HMODULE>(source);
		std::string creatorFunctionName = std::format("create{}TaskCXXInstance", name);

		if (task_broker::CreateTaskExecutorSignature creator = utility::load<task_broker::CreateTaskExecutorSignature>(module, creatorFunctionName))
		{
			std::filesystem::path sourcePath = utility::getPathToLibrary(module);

			if (Log::isValid())
			{
				Log::info("Found {} in {}", "LogWebFrameworkInitialization", creatorFunctionName, sourcePath.empty() ? "current" : sourcePath.string());
			}

			return std::make_unique<task_broker::CXXTaskExecutor>(module, creator());
		}

		throw std::runtime_error(std::format("Can't find {}", name));
	}

	void* CXXRuntime::createExecutorSettings(const void* implementation) const
	{
		return const_cast<void*>(implementation);
	}

	void* CXXRuntime::createHTTPRequest(framework::interfaces::IHttpRequest* request) const
	{
		return request;
	}

	void* CXXRuntime::createHTTPResponse(framework::interfaces::IHttpResponse* response) const
	{
		return response;
	}

	void CXXRuntime::initializeWebFramework(const utility::LoadSource& source, std::string_view libraryPath)
	{
		utility::load<InitializeWebFrameworkInExecutor>(std::get<HMODULE>(source), "initializeWebFrameworkCXX")(libraryPath.data());
	}

	std::optional<std::string> CXXRuntime::loadSource(std::string_view pathToSource, utility::LoadSource& source)
	{
		HMODULE temp = utility::loadLibrary(pathToSource);

		if (!temp)
		{
			return std::optional<std::string>(::exceptions::missingOtherDLLs);
		}

		if (!utility::load<InitializeWebFrameworkInExecutor>(temp, "initializeWebFrameworkCXX"))
		{
			return std::optional<std::string>(::exceptions::notCXXLibrary);
		}

		source = temp;

		return std::nullopt;
	}
}
