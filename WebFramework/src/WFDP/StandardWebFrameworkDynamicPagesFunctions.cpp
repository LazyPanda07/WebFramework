#include "StandardWebFrameworkDynamicPagesFunctions.h"

#include <fstream>

#include <Exceptions/FileDoesNotExistException.h>

namespace framework
{
	std::string print(const std::vector<std::string>& arguments)
	{
		std::string result;

		for (const auto& i : arguments)
		{
			result += i + ' ';
		}

		result.pop_back();

		return result;
	}

	std::string include(const std::vector<std::string>& arguments, const std::string& pathToTemplates)
	{
		std::string result;
		std::string tem;
		const std::filesystem::path filePath(pathToTemplates + '/' + arguments[0]);

		if (!std::filesystem::exists(filePath))
		{
			throw file_manager::exceptions::FileDoesNotExistException(filePath.string());
		}

		result.reserve(std::filesystem::file_size(filePath));

		std::ifstream in(filePath);

		while (std::getline(in, tem))
		{
			result += tem + '\n';
		}

		in.close();

		return result;
	}

	std::string forWFDP(const std::vector<std::string>& arguments, const utility::strings::string_based_unordered_map<std::unique_ptr<DynamicFunction>>& dynamicPagesFunctions)
	{
		int64_t start = stoll(arguments[0]);
		int64_t end = stoll(arguments[1]);
		DynamicFunction& repeatableFunction = *dynamicPagesFunctions.at(arguments[2]);
		int64_t step = 1;
		std::string result;
		
		if (arguments.size() == 4)
		{
			step = std::stoll(arguments[3]);
		}

		for (int64_t i = start; i < end; i += step)
		{
			result += repeatableFunction({ std::to_string(i) });
		}

		return result;
	}
}
