#include "StandardWebFrameworkDynamicPagesFunctions.h"

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
			throw file_manager::exceptions::FileDoesNotExistException(filePath.string());
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

	string forWFDP(const vector<string>& arguments, const unordered_map<string, function<string(const vector<string>&)>>& dynamicPagesFunctions)
	{
		int64_t start = stoll(arguments[0]);
		int64_t end = stoll(arguments[1]);
		const function<string(const vector<string>&)> repeatableFunction = dynamicPagesFunctions.at(arguments[2]);
		int64_t step = 1;
		string result;
		
		if (arguments.size() == 4)
		{
			step = stoll(arguments[3]);
		}

		for (int64_t i = start; i < end; i += step)
		{
			result += repeatableFunction({ to_string(i) });
		}

		return result;
	}
}
