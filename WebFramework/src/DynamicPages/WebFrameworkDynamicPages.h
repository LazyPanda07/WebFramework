#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

namespace framework
{
	/// <summary>
	/// Providing special syntax for .html files for adding data dynamically
	/// </summary>
	class WebFrameworkDynamicPages
	{
	private:
		/// <summary>
		/// Utility struct for calling standard WebFrameworkDynamicPages functions
		/// </summary>
		struct executionUnit
		{
			std::string functionName;
			std::vector<std::string> arguments;

			executionUnit(std::string&& functionName, std::vector<std::string>&& arguments) noexcept;
		};

	private:
		static const std::unordered_map<std::string, std::function<std::string(const std::vector<std::string>&)>> dynamicPagesFunctions;

	private:
		/// <summary>
		/// Clear all spaces
		/// </summary>
		/// <param name="code">section with special syntax</param>
		static void clear(std::string& code);

		/// <summary>
		/// Replace all variables with their names
		/// </summary>
		/// <param name="variables">name - value</param>
		/// <param name="code">section with special syntax</param>
		/// <returns>string after all variable replaces</returns>
		static std::string insertVariables(const std::unordered_map<std::string_view, std::string>& variables, std::string code);

		/// <summary>
		/// Split all sections in functions calling order
		/// </summary>
		/// <param name="code">section with special syntax</param>
		/// <returns>functions for calling</returns>
		static std::vector<executionUnit> preExecute(const std::string& code);

		/// <summary>
		/// Call all function in order
		/// </summary>
		/// <param name="codes">section with special syntax</param>
		/// <returns>string after all replaces</returns>
		std::string execute(const std::vector<executionUnit>& codes);

	public:
		WebFrameworkDynamicPages() = default;

		/// <summary>
		/// Run interpreter
		/// </summary>
		/// <param name="variables">$key$ - value</param>
		/// <param name="source">data string</param>
		void run(const std::unordered_map<std::string_view, std::string>& variables, std::string& source);

		~WebFrameworkDynamicPages() = default;
	};
}
