#pragma once

#include <string>
#include <vector>

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
	/// <param name="arguments">take file path from arguments[0]</param>
	/// <returns>all data from another file</returns>
	std::string include(const std::vector<std::string>& arguments);
}
