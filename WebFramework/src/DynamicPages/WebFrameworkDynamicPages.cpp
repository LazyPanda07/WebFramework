#include "WebFrameworkDynamicPages.h"

#include "Exceptions/DynamicPagesSyntaxException.h"
#include "StandardWebFrameworkDynamicPagesFunctions.h"

#pragma warning(disable: 26800)

using namespace std;

namespace framework
{
	WebFrameworkDynamicPages::ExecutionUnit::ExecutionUnit(string&& functionName, vector<string>&& arguments) noexcept :
		functionName(move(functionName)),
		arguments(move(arguments))
	{

	}

	void WebFrameworkDynamicPages::clear(string& code)
	{
		code.erase(remove_if(code.begin(), code.end(), [](const char& c) { return iscntrl(c) || isspace(c); }), code.end());
	}

	string WebFrameworkDynamicPages::insertVariables(const unordered_map<string, string>& variables, string code)
	{
		size_t changeVariableStart = code.find('$');

		while (changeVariableStart != string::npos)
		{
			changeVariableStart++;

			size_t changeVariableEnd = code.find('$', changeVariableStart);

			if (changeVariableEnd == string::npos)
			{
				throw exceptions::DynamicPagesSyntaxException(::exceptions::variableDeclarationSyntaxError);
			}

			const string& variable = variables.at(string(code.data() + changeVariableStart, changeVariableEnd - changeVariableStart));

			code.replace(code.begin() + changeVariableStart - 1, code.begin() + changeVariableEnd + 1, variable);

			changeVariableStart = code.find('$');
		}

		return code;
	}

	vector<WebFrameworkDynamicPages::ExecutionUnit> WebFrameworkDynamicPages::preExecute(const string& code)
	{
		vector<ExecutionUnit> result;
		size_t startLine = 0;
		size_t endLine = code.find(';');

		if (endLine == string::npos)
		{
			throw exceptions::DynamicPagesSyntaxException(::exceptions::missingSemicolonSyntaxError);
		}

		result.reserve(count(code.begin(), code.end(), ';'));

		while (endLine != string::npos)
		{
			endLine++;

			const string_view line(code.data() + startLine, endLine - startLine);
			const size_t openBracket = line.find('(');
			string functionName(line.substr(0, openBracket));
			vector<string> arguments;
			istringstream is(string(line.data() + openBracket + 1, line.find(')') - openBracket - 1));
			string tem;

			arguments.reserve(count(line.begin(), line.end(), ',') + 1);

			while (is >> tem)
			{
				arguments.push_back(move(tem));
			}

			result.emplace_back(move(functionName), move(arguments));

			startLine = endLine;
			endLine = code.find(';', endLine + 1);
		}

		return result;
	}

	string WebFrameworkDynamicPages::execute(const vector<ExecutionUnit>& codes)
	{
		string result;

		for (const auto& i : codes)
		{
			try
			{
				result += dynamicPagesFunctions.at(i.functionName)(i.arguments);
			}
			catch (const exception& e)
			{
				Log::error("WebFrameworkDynamicPages execute exception: {}", e.what());

				throw;
			}
		}

		return result;
	}

	WebFrameworkDynamicPages::WebFrameworkDynamicPages(const string& pathToTemplates) :
		pathToTemplates(pathToTemplates)
	{
		dynamicPagesFunctions.insert({ "print", print });
		dynamicPagesFunctions.insert({ "include", bind(include, placeholders::_1, pathToTemplates) });
		dynamicPagesFunctions.insert({ "for", bind(forWFDP, placeholders::_1, ref(dynamicPagesFunctions)) });
	}

	void WebFrameworkDynamicPages::run(const unordered_map<string, string>& variables, string& source)
	{
		size_t nextSectionStart = source.find("{%");

		while (nextSectionStart != string::npos)
		{
			size_t nextSectionEnd = source.find("%}", nextSectionStart);

			if (nextSectionEnd == string::npos)
			{
				throw exceptions::DynamicPagesSyntaxException(::exceptions::sectionDeclarationSyntaxError);
			}

			string code(source.begin() + nextSectionStart + 2, source.begin() + nextSectionEnd);

			clear(code);

			if (variables.size())
			{
				code = insertVariables(variables, code);
			}

			replace(code.begin(), code.end(), ',', ' ');

			source.replace(source.begin() + nextSectionStart, source.begin() + nextSectionEnd + 2, this->execute(preExecute(code)));

			nextSectionStart = source.find("{%", nextSectionStart + 1);
		}

		if (source.find("{%") != string::npos)
		{
			this->run(variables, source);
		}
	}

	void WebFrameworkDynamicPages::registerDynamicFunction(const string& functionName, function<string(const vector<string>&)>&& function)
	{
		dynamicPagesFunctions.insert(make_pair(functionName, move(function)));
	}

	void WebFrameworkDynamicPages::unregisterDynamicFunction(const string& functionName)
	{
		dynamicPagesFunctions.erase(functionName);
	}

	bool WebFrameworkDynamicPages::isDynamicFunctionRegistered(const string& functionName)
	{
		return dynamicPagesFunctions.find(functionName) != dynamicPagesFunctions.end();
	}

	const string& WebFrameworkDynamicPages::getPathToTemplates() const
	{
		return pathToTemplates;
	}
}
