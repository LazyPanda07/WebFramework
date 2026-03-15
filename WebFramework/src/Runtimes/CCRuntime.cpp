#include "Runtimes/CCRuntime.h"

#include "Executors/CCExecutor.h"
#include "Utility/DynamicLibraries.h"
#include "Framework/WebFrameworkConstants.h"
#include "TaskBroker/TaskExecutors/CCTaskExecutor.h"
#include "Utility/Utils.h"

namespace framework::runtime
{
	void CCRuntime::initializeWebFramework(const utility::LoadSource& source, std::string_view libraryPath)
	{
		utility::load<InitializeWebFrameworkInExecutor>(std::get<HMODULE>(source), "initializeWebFrameworkCC")(libraryPath.data());
	}

	void CCRuntime::finishInitialization()
	{

	}

	bool CCRuntime::loadExecutor(std::string_view name, std::string_view route, const utility::LoadSource& source)
	{
		if (!std::holds_alternative<HMODULE>(source))
		{
			return false;
		}

		HMODULE module = std::get<HMODULE>(source);
		std::string creatorFunctionName = std::format("create{}CCInstance", name);

		if (CreateExecutorSignature creator = utility::load<CreateExecutorSignature>(module, creatorFunctionName))
		{
			std::filesystem::path sourcePath = utility::getPathToLibrary(module);

			if (Log::isValid())
			{
				Log::info<logging::message::foundExecutor, logging::category::ccRuntime>(creatorFunctionName, sourcePath.empty() ? "current" : sourcePath.string(), route.empty() ? R"("")" : route);
			}

			creators.emplace(name, std::make_tuple(module, creator));

			return true;
		}

		return false;
	}

	std::unique_ptr<Executor> CCRuntime::createExecutor(std::string_view name) const
	{
		auto it = creators.find(name);

		if (it == creators.end())
		{
			utility::logAndThrowException<logging::message::cantFindExecutor, logging::category::ccRuntime>(name);
		}

		const auto& [_, creatorData] = *it;
		const auto& [module, creator] = creatorData;

		return std::make_unique<CCExecutor>(module, creator(), name);
	}

	std::unique_ptr<task_broker::TaskExecutor> CCRuntime::createTaskExecutor(std::string_view name, const utility::LoadSource& source) const
	{
		if (!std::holds_alternative<HMODULE>(source))
		{
			return nullptr;
		}

		HMODULE module = std::get<HMODULE>(source);
		std::string creatorFunctionName = std::format("create{}TaskCCInstance", name);

		if (CreateExecutorSignature creator = utility::load<CreateExecutorSignature>(module, creatorFunctionName))
		{
			std::filesystem::path sourcePath = utility::getPathToLibrary(module);

			if (Log::isValid())
			{
				Log::info<logging::message::foundTaskExecutor, logging::category::ccRuntime>(creatorFunctionName, sourcePath.empty() ? "current" : sourcePath.string());
			}

			return std::make_unique<task_broker::CCTaskExecutor>(module, creator(), name);
		}

		utility::logAndThrowException<logging::message::cantFindTaskExecutor, logging::category::ccRuntime>(name);
	}

	void* CCRuntime::createExecutorSettings(const void* implementation) const
	{
		return const_cast<void*>(implementation);
	}

	void* CCRuntime::createHTTPRequest(framework::interfaces::IHttpRequest* request) const
	{
		return request;
	}

	void* CCRuntime::createHTTPResponse(framework::interfaces::IHttpResponse* response) const
	{
		return response;
	}

	std::optional<std::string> CCRuntime::loadSource(std::string_view pathToSource, utility::LoadSource& source)
	{
		HMODULE temp = utility::loadLibrary(pathToSource);

		if (!temp)
		{
			return std::optional<std::string>(::exceptions::missingOtherDLLs);
		}

		if (!utility::load<InitializeWebFrameworkInExecutor>(temp, "initializeWebFrameworkCC"))
		{
			return std::optional<std::string>(::exceptions::notCCLibrary);
		}

		source = temp;

		return std::nullopt;
	}
}
