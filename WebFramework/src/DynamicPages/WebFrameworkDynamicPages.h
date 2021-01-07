#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

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
		static const std::unordered_map<std::string, std::function<std::string(const std::vector<std::string>&)>> dynamicPagesFunctions;

	private:
		static void clear(std::string& code);

		static std::string insertVariables(const std::unordered_map<std::string_view, std::string>& variables, std::string code);

		static std::vector<executionUnit> preExecute(const std::string& code);

		std::string execute(const std::vector<executionUnit>& codes);

	public:
		WebFrameworkDynamicPages() = default;

		void run(const std::unordered_map<std::string_view, std::string>& variables, std::string& source);

		~WebFrameworkDynamicPages() = default;
	};
}
