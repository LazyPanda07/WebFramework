#pragma once

#include "Import/WebFrameworkCore.h"

#include "Strings.h"

namespace framework
{
	/// <summary>
	/// Print data to .wfdp file
	/// </summary>
	/// <param name="arguments">for printing</param>
	/// <returns>arguments with spaces</returns>
	std::string print(const std::vector<std::string>& arguments);

	/// <summary>
	/// Include another file
	/// </summary>
	/// <param name="arguments">take file name from arguments[0]</param>
	/// <returns>all data from another file</returns>
	/// <exception cref="file_manager::exceptions::FileDoesNotExistException"></exception>
	std::string include(const std::vector<std::string>& arguments, const std::string& pathToTemplates);

	/// @brief For cycle
	/// @param arguments arguments[0] - start index, arguments[1] - index after end, arguments[2] - function name, arguments[3] - cycle step(optional parameter)
	/// @return All data from function
	std::string forWFDP(const std::vector<std::string>& arguments, const utility::strings::string_based_unordered_map<std::function<std::string(const std::vector<std::string>&)>>& dynamicPagesFunctions);
}
