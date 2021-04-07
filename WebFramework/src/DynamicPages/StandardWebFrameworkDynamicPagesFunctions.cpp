#include "StandardWebFrameworkDynamicPagesFunctions.h"

#include <fstream>
#include <filesystem>

#include "Exceptions/FileDoesNotExistException.h"
#include "Utility/StringConversion.h"

using namespace std;

namespace framework
{
	string print(const vector<string>& arguments)
	{
		string result;

		for (const auto& i : arguments)
		{
			result += i + ' ';
		}

		result.pop_back();

		return result;
	}

	string include(const vector<string>& arguments, const string& pathToTemplates)
	{
		string result;
		string tem;
		const filesystem::path filePath(pathToTemplates + '/' + arguments[0]);

		if (!filesystem::exists(filePath))
		{
			throw exceptions::FileDoesNotExistException(filePath.string());
		}

		result.reserve(filesystem::file_size(filePath));

		ifstream in(filePath);

		while (getline(in, tem))
		{
			result += tem + '\n';
		}

		in.close();

		return result;
	}

	string forImplementation(const vector<string>& arguments, const unordered_map<string, function<string(const vector<string>&)>>& dynamicPagesFunctions)
	{
		int64_t start = stoll(arguments[0]);
		int64_t end = stoll(arguments[1]);
		string result;
		const function<string(const vector<string>&)> repeatableFunction = dynamicPagesFunctions.at(arguments[2]);

		for (int64_t i = start; i < end; i++)
		{
			result += repeatableFunction({ to_string(i) });
		}

		return result;
	}
}