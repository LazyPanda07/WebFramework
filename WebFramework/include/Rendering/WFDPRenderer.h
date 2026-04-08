#pragma once

#include <functional>
#include <filesystem>
#include <span>

#include <Strings.h>
#include <JsonObject.h>

#include "Framework/WebFrameworkPlatform.h"
#include "WebInterfaces/IHttpRequest.h"
#include "WFDP/DynamicFunction.h"

namespace framework
{
	class WFDPRenderer
	{
	private:
		struct ExecutionUnit
		{
			std::string functionName;
			const json::JsonObject& sharedArguments;
			json::JsonObject defaultArguments;

			ExecutionUnit(std::string_view functionName, const json::JsonObject& sharedArguments);
		};

	private:
		::utility::strings::string_based_unordered_map<std::unique_ptr<DynamicFunction>> dynamicPagesFunctions;
		const std::filesystem::path pathToTemplates;

	private:
		static std::vector<ExecutionUnit> parse(std::string_view code, const json::JsonObject& sharedArguments, bool checks);

		std::string execute(const std::vector<ExecutionUnit>& codes);

	public:
		WFDPRenderer(const std::filesystem::path& pathToTemplates);

		void run(const void* arguments, std::string& source);

		void registerDynamicFunction(std::string_view functionName, std::unique_ptr<DynamicFunction>&& dynamicFunction);

		void unregisterDynamicFunction(std::string_view functionName);

		bool isDynamicFunctionRegistered(std::string_view functionName);

		const std::filesystem::path& getPathToTemplates() const;

		~WFDPRenderer() = default;
	};
}
