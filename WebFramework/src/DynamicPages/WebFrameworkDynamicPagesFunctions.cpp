#include "WebFrameworkDynamicPagesFunctions.h"

#include <fstream>
#include <filesystem>

#include "Exceptions/FileDoesNotExistException.h"

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
}
