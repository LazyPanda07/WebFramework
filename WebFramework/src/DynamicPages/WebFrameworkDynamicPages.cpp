#include "WebFrameworkDynamicPages.h"

#include "Strings.h"
#include "Log.h"

#include "Exceptions/DynamicPagesSyntaxException.h"
#include "StandardWebFrameworkDynamicPagesFunctions.h"

#pragma warning(disable: 26800)

using namespace std;

static constexpr string_view argumentsDelimiter = "</arg>";

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

	void WebFrameworkDynamicPages::separateArguments(string& code)
	{
		bool stringArgument = false;

		for (size_t i = 0; i < code.size(); i++)
		{
			switch (code[i])
			{
			case '"':
				stringArgument = !stringArgument;

				break;

			case ',':
				if (stringArgument)
				{
					continue;
				}

				code.replace(i, 1, argumentsDelimiter);

				break;

			default:
				break;
			}
		}
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

			string_view line(code.data() + startLine, endLine - startLine);
			size_t openBracket = line.find('(');
			string functionName(line.substr(0, openBracket));

			result.emplace_back
			(
				move(functionName),
				utility::strings::split(string_view(line.data() + openBracket + 1, line.find(')') - openBracket - 1), argumentsDelimiter)
			);

			startLine = endLine;
			endLine = code.find(';', endLine + 1);
		}

		return result;
	}

	string WebFrameworkDynamicPages::execute(const vector<ExecutionUnit>& codes)
	{
		string result;

		for (const auto& [functionName, arguments] : codes)
		{
			try
			{
				result += dynamicPagesFunctions.at(functionName)(arguments);
			}
			catch (const exception& e)
			{
				if (Log::isValid())
				{
					Log::error("WebFrameworkDynamicPages execute exception: {}", "LogWebFrameworkDynamicPages", e.what());
				}

				throw;
			}
		}

		return result;
	}

	WebFrameworkDynamicPages::WebFrameworkDynamicPages(const filesystem::path& pathToTemplates) :
		pathToTemplates(pathToTemplates)
	{
		dynamicPagesFunctions.try_emplace("print", print);
		dynamicPagesFunctions.try_emplace("include", bind(include, placeholders::_1, pathToTemplates.string()));
		dynamicPagesFunctions.try_emplace("for", bind(forWFDP, placeholders::_1, ref(dynamicPagesFunctions)));
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

			WebFrameworkDynamicPages::clear(code);

			WebFrameworkDynamicPages::separateArguments(code);

			if (variables.size())
			{
				code = WebFrameworkDynamicPages::insertVariables(variables, code);
			}

			source.replace(source.begin() + nextSectionStart, source.begin() + nextSectionEnd + 2, this->execute(WebFrameworkDynamicPages::preExecute(code)));

			nextSectionStart = source.find("{%", nextSectionStart + 1);
		}

		if (source.find("{%") != string::npos)
		{
			this->run(variables, source);
		}
	}

	void WebFrameworkDynamicPages::registerDynamicFunction(const string& functionName, function<string(const vector<string>&)>&& function)
	{
		dynamicPagesFunctions.try_emplace(functionName, move(function));
	}

	void WebFrameworkDynamicPages::unregisterDynamicFunction(const string& functionName)
	{
		dynamicPagesFunctions.erase(functionName);
	}

	bool WebFrameworkDynamicPages::isDynamicFunctionRegistered(const string& functionName)
	{
		return dynamicPagesFunctions.find(functionName) != dynamicPagesFunctions.end();
	}

	const filesystem::path& WebFrameworkDynamicPages::getPathToTemplates() const
	{
		return pathToTemplates;
	}
}
