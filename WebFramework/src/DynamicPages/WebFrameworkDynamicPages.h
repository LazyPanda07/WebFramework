#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "WebFrameworkConstants.h"

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
		std::unordered_map<std::string, std::function<std::string(const std::vector<std::string>&)>> dynamicPagesFunctions;
		const std::string pathToTemplates;

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
		/// <exception cref="framework::exceptions::DynamicPagesSyntaxException"></exception>
		static std::string insertVariables(const smartPointer<std::unordered_map<std::string_view, std::string>>& variables, std::string code);

		/// <summary>
		/// Split all sections in functions calling order
		/// </summary>
		/// <param name="code">section with special syntax</param>
		/// <returns>functions for calling</returns>
		/// <exception cref="framework::exceptions::DynamicPagesSyntaxException"></exception>
		static std::vector<executionUnit> preExecute(const std::string& code);

		/// <summary>
		/// Call all function in order
		/// </summary>
		/// <param name="codes">section with special syntax</param>
		/// <returns>string after all replaces</returns>
		/// /// <exception cref="std::exception"></exception>
		std::string execute(const std::vector<executionUnit>& codes);

	public:
		/// <summary>
		/// Construct WebFrameworkDynamicPages
		/// </summary>
		/// <param name="pathToTemplates">path to templates folder</param>
		WebFrameworkDynamicPages(const std::string& pathToTemplates);

		/// <summary>
		/// Run interpreter
		/// </summary>
		/// <param name="variables">$key$ - value</param>
		/// <param name="source">data string</param>
		/// <exception cref="framework::exceptions::DynamicPagesSyntaxException"></exception>
		/// <exception cref="std::exception"></exception>
		void run(const smartPointer<std::unordered_map<std::string_view, std::string>>& variables, std::string& source);

		/// @brief Add new function in .wfdp interpreter
		/// @param functionName Name of new function
		/// @param function Function implementation
		void registerDynamicFunction(const std::string& functionName, std::function<std::string(const std::vector<std::string>&)>&& function);

		/// @brief Remove function from .wfdp interpreter
		/// @param functionName Name of function
		void unregisterDynamicFunction(const std::string& functionName);

		/// @brief Check if function is registered
		/// @param functionName Name of function
		/// @return true if function is registered, false otherwise
		bool isDynamicFunctionRegistered(const std::string& functionName);

		/// <summary>
		/// Getter for pathToTemplates
		/// </summary>
		/// <returns>pathToTemplates</returns>
		const std::string& getPathToTemplates() const;

		~WebFrameworkDynamicPages() = default;
	};
}
