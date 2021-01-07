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
}
