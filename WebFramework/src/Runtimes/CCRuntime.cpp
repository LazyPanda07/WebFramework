#include "CCRuntime.h"

#include <Log.h>

#include "Executors/CCExecutor.h"
#include "Utility/DynamicLibraries.h"
#include "Framework/WebFrameworkConstants.h"

namespace framework::runtime
{
	void CCRuntime::finishInitialization()
	{

	}

	bool CCRuntime::loadExecutor(std::string_view name, const utility::LoadSource& source)
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
				Log::info("Found {} in {}", "LogRuntime", creatorFunctionName, sourcePath.empty() ? "current" : sourcePath.string());
			}

			creators.emplace(name, std::make_tuple(module, creator));

			return true;
		}

		return false;
	}

	std::unique_ptr<BaseExecutor> CCRuntime::createExecutor(std::string_view name) const
	{
		auto it = creators.find(name);

		if (it == creators.end())
		{
			throw std::runtime_error(std::format("Can't find executor with name {}", name));
		}

		const auto& [_, creatorData] = *it;
		const auto& [module, creator] = creatorData;

		return std::make_unique<CCExecutor>(module, creator(), name);
	}

	void* CCRuntime::createExecutorSettings(const void* implementation) const
	{
		return const_cast<void*>(implementation);
	}

	void* CCRuntime::createHTTPRequest(framework::interfaces::IHTTPRequest* request) const
	{
		return request;
	}

	void* CCRuntime::createHTTPResponse(framework::interfaces::IHTTPResponse* response) const
	{
		return response;
	}

	void CCRuntime::initializeWebFramework(const utility::LoadSource& source, std::string_view libraryPath)
	{
		utility::load<InitializeWebFrameworkInExecutor>(std::get<HMODULE>(source), "initializeWebFrameworkCC")(libraryPath.data());
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
