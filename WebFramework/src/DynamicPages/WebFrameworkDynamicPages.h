#pragma once

#include <functional>
#include <filesystem>
#include <span>

#include "Framework/WebFrameworkPlatform.h"

#include "Strings.h"
#include "WebInterfaces/IHTTPRequest.h"

namespace framework
{
	class WebFrameworkDynamicPages
	{
	private:
		struct ExecutionUnit
		{
			std::string functionName;
			std::vector<std::string> arguments;

			ExecutionUnit(std::string&& functionName, std::vector<std::string>&& arguments) noexcept;
		};

	private:
		::utility::strings::string_based_unordered_map<std::function<std::string(const std::vector<std::string>&)>> dynamicPagesFunctions;
		const std::filesystem::path pathToTemplates;

	private:
		static void clear(std::string& code);

		static void separateArguments(std::string& code);

		static std::string insertVariables(std::span<const interfaces::CVariable> variables, std::string code);

		static std::vector<ExecutionUnit> preExecute(std::string_view code);

		std::string execute(const std::vector<ExecutionUnit>& codes);

	public:
		WebFrameworkDynamicPages(const std::filesystem::path& pathToTemplates);

		void run(std::span<const interfaces::CVariable> variables, std::string& source);

		void registerDynamicFunction(std::string_view functionName, std::function<std::string(const std::vector<std::string>&)>&& function);

		void unregisterDynamicFunction(std::string_view functionName);

		bool isDynamicFunctionRegistered(std::string_view functionName);

		const std::filesystem::path& getPathToTemplates() const;

		~WebFrameworkDynamicPages() = default;
	};
}
