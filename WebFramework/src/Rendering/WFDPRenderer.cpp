#include "WFDPRenderer.h"

#include "Strings.h"
#include "Log.h"

#include "Exceptions/DynamicPagesSyntaxException.h"
#include "StandardWebFrameworkDynamicPagesFunctions.h"
#include "Framework/WebFrameworkConstants.h"

#pragma warning(disable: 26800)

using namespace std;

static constexpr string_view argumentsDelimiter = "</arg>";

namespace framework
{
	WFDPRenderer::ExecutionUnit::ExecutionUnit(string&& functionName, vector<string>&& arguments) noexcept :
		functionName(move(functionName)),
		arguments(move(arguments))
	{

	}

	void WFDPRenderer::clear(string& code)
	{
		code.erase(remove_if(code.begin(), code.end(), [](const char& c) { return iscntrl(c) || isspace(c); }), code.end());
	}

	void WFDPRenderer::separateArguments(string& code)
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

	string WFDPRenderer::insertVariables(span<const interfaces::CVariable> variables, string code)
	{
		auto findVariable = [&variables](string_view name) -> string_view
			{
				auto it = ranges::find_if(variables, [name](const auto& variable) { return variable.name == name; });

				if (it == variables.end())
				{
					throw runtime_error(format("No variable: {}", name));
				}

				return it->value;
			};

		size_t changeVariableStart = code.find('$');

		while (changeVariableStart != string::npos)
		{
			changeVariableStart++;

			size_t changeVariableEnd = code.find('$', changeVariableStart);

			if (changeVariableEnd == string::npos)
			{
				throw exceptions::DynamicPagesSyntaxException(::exceptions::variableDeclarationSyntaxError);
			}

			string_view variableName(code.data() + changeVariableStart, changeVariableEnd - changeVariableStart);

			code.replace(code.begin() + changeVariableStart - 1, code.begin() + changeVariableEnd + 1, findVariable(variableName));

			changeVariableStart = code.find('$');
		}

		return code;
	}

	vector<WFDPRenderer::ExecutionUnit> WFDPRenderer::preExecute(string_view code)
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

	string WFDPRenderer::execute(const vector<ExecutionUnit>& codes)
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
					Log::error("WFDPRenderer execute exception: {}", "LogWebFrameworkDynamicPages", e.what());
				}

				throw;
			}
		}

		return result;
	}

	WFDPRenderer::WFDPRenderer(const filesystem::path& pathToTemplates) :
		pathToTemplates(pathToTemplates)
	{
		dynamicPagesFunctions.try_emplace("print", print);
		dynamicPagesFunctions.try_emplace("include", bind(include, placeholders::_1, pathToTemplates.string()));
		dynamicPagesFunctions.try_emplace("for", bind(forWFDP, placeholders::_1, ref(dynamicPagesFunctions)));
	}

	void WFDPRenderer::run(span<const interfaces::CVariable> variables, string& source)
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

			WFDPRenderer::clear(code);

			WFDPRenderer::separateArguments(code);

			if (variables.size())
			{
				code = WFDPRenderer::insertVariables(variables, code);
			}

			source.replace(source.begin() + nextSectionStart, source.begin() + nextSectionEnd + 2, this->execute(WFDPRenderer::preExecute(code)));

			nextSectionStart = source.find("{%", nextSectionStart + 1);
		}

		if (source.find("{%") != string::npos)
		{
			this->run(variables, source);
		}
	}

	void WFDPRenderer::registerDynamicFunction(string_view functionName, function<string(const vector<string>&)>&& function)
	{
		dynamicPagesFunctions.emplace(functionName, move(function));
	}

	void WFDPRenderer::unregisterDynamicFunction(string_view functionName)
	{
		if (auto it = dynamicPagesFunctions.find(functionName); it != dynamicPagesFunctions.end())
		{
			dynamicPagesFunctions.erase(it);
		}
	}

	bool WFDPRenderer::isDynamicFunctionRegistered(string_view functionName)
	{
		return dynamicPagesFunctions.find(functionName) != dynamicPagesFunctions.end();
	}

	const filesystem::path& WFDPRenderer::getPathToTemplates() const
	{
		return pathToTemplates;
	}
}
