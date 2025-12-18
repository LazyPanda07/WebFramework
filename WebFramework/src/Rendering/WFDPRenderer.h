#pragma once

#include <functional>
#include <filesystem>
#include <span>

#include <Strings.h>

#include "Framework/WebFrameworkPlatform.h"
#include "WebInterfaces/IHTTPRequest.h"
#include "WFDP/DynamicFunction.h"

namespace framework
{
	class WFDPRenderer
	{
	private:
		struct ExecutionUnit
		{
			std::string functionName;
			std::vector<std::string> arguments;

			ExecutionUnit(std::string&& functionName, std::vector<std::string>&& arguments) noexcept;
		};

	private:
		::utility::strings::string_based_unordered_map<std::unique_ptr<DynamicFunction>> dynamicPagesFunctions;
		const std::filesystem::path pathToTemplates;

	private:
		static void clear(std::string& code);

		static void separateArguments(std::string& code);

		static std::string insertVariables(std::span<const interfaces::CVariable> variables, std::string code);

		static std::vector<ExecutionUnit> preExecute(std::string_view code);

		std::string execute(const std::vector<ExecutionUnit>& codes);

	public:
		WFDPRenderer(const std::filesystem::path& pathToTemplates);

		void run(std::span<const interfaces::CVariable> variables, std::string& source);

		void registerDynamicFunction(std::string_view functionName, std::unique_ptr<DynamicFunction>&& dynamicFunction);

		void unregisterDynamicFunction(std::string_view functionName);

		bool isDynamicFunctionRegistered(std::string_view functionName);

		const std::filesystem::path& getPathToTemplates() const;

		~WFDPRenderer() = default;
	};
}
