#pragma once

#include <memory>
#include <filesystem>

#include <Strings.h>

#include "Framework/WebFrameworkPlatform.h"
#include "DynamicFunction.h"

namespace framework
{
	/// <summary>
	/// Print data to .wfdp file
	/// </summary>
	/// <param name="arguments">for printing</param>
	/// <returns>arguments with spaces</returns>
	std::unique_ptr<DynamicFunction> createPrintFunction();

	/// <summary>
	/// Include another file
	/// </summary>
	/// <param name="arguments">take file name from arguments[0]</param>
	/// <returns>all data from another file</returns>
	/// <exception cref="file_manager::exceptions::FileDoesNotExistException"></exception>
	std::unique_ptr<DynamicFunction> createIncludeFunction(const std::filesystem::path& pathToTemplates);

	/// @brief For cycle
	/// @param arguments arguments[0] - start index, arguments[1] - index after end, arguments[2] - function name, arguments[3] - cycle step(optional parameter)
	/// @return All data from function
	std::unique_ptr<DynamicFunction> createForFunction(const ::utility::strings::string_based_unordered_map<std::unique_ptr<DynamicFunction>>& dynamicPagesFunctions);
}
