#pragma once

#include "headers.h"

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
		std::unordered_map<std::string, std::function<std::string(const std::vector<std::string>&)>> dynamicPagesFunctions;
		const std::string pathToTemplates;

	private:
		static void clear(std::string& code);

		static void separateArguments(std::string& code);

		static std::string insertVariables(const std::unordered_map<std::string, std::string>& variables, std::string code);

		static std::vector<ExecutionUnit> preExecute(const std::string& code);

		std::string execute(const std::vector<ExecutionUnit>& codes);

	public:
		WebFrameworkDynamicPages(const std::string& pathToTemplates);

		void run(const std::unordered_map<std::string, std::string>& variables, std::string& source);

		void registerDynamicFunction(const std::string& functionName, std::function<std::string(const std::vector<std::string>&)>&& function);

		void unregisterDynamicFunction(const std::string& functionName);

		bool isDynamicFunctionRegistered(const std::string& functionName);

		const std::string& getPathToTemplates() const;

		~WebFrameworkDynamicPages() = default;
	};
}
