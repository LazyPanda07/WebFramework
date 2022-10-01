#pragma once

#include "headers.h"

namespace framework
{
	class WebFrameworkDynamicPages
	{
	private:
		struct executionUnit
		{
			std::string functionName;
			std::vector<std::string> arguments;

			executionUnit(std::string&& functionName, std::vector<std::string>&& arguments) noexcept;
		};

	private:
		std::unordered_map<std::string, std::function<std::string(const std::vector<std::string>&)>> dynamicPagesFunctions;
		const std::string pathToTemplates;

	private:
		static void clear(std::string& code);

		static std::string insertVariables(const std::unordered_map<std::string_view, std::string>& variables, std::string code);

		static std::vector<executionUnit> preExecute(const std::string& code);

		std::string execute(const std::vector<executionUnit>& codes);

	public:
		WebFrameworkDynamicPages(const std::string& pathToTemplates);

		void run(const std::unordered_map<std::string_view, std::string>& variables, std::string& source);

		void registerDynamicFunction(const std::string& functionName, std::function<std::string(const std::vector<std::string>&)>&& function);

		void unregisterDynamicFunction(const std::string& functionName);

		bool isDynamicFunctionRegistered(const std::string& functionName);

		const std::string& getPathToTemplates() const;

		~WebFrameworkDynamicPages() = default;
	};
}
