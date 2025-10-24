#include "WFDPRenderer.h"

#include <Log.h>

#include "Exceptions/DynamicPagesSyntaxException.h"
#include "WFDP/StandardWebFrameworkDynamicPagesFunctions.h"
#include "WFDP/CXXDynamicFunction.h"
#include "Framework/WebFrameworkConstants.h"
#include "ExecutorsConstants.h"

#pragma warning(disable: 26800)

static constexpr std::string_view argumentsDelimiter = "</arg>";

namespace framework
{
	WFDPRenderer::ExecutionUnit::ExecutionUnit(std::string&& functionName, std::vector<std::string>&& arguments) noexcept :
		functionName(std::move(functionName)),
		arguments(std::move(arguments))
	{

	}

	void WFDPRenderer::clear(std::string& code)
	{
		code.erase(std::remove_if(code.begin(), code.end(), [](const char& c) { return std::iscntrl(c) || std::isspace(c); }), code.end());
	}

	void WFDPRenderer::separateArguments(std::string& code)
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

	std::string WFDPRenderer::insertVariables(std::span<const interfaces::CVariable> variables, std::string code)
	{
		auto findVariable = [&variables](std::string_view name) -> std::string_view
			{
				auto it = std::ranges::find_if(variables, [name](const auto& variable) { return variable.name == name; });

				if (it == variables.end())
				{
					throw std::runtime_error(std::format("No variable: {}", name));
				}

				return it->value;
			};

		size_t changeVariableStart = code.find('$');

		while (changeVariableStart != std::string::npos)
		{
			changeVariableStart++;

			size_t changeVariableEnd = code.find('$', changeVariableStart);

			if (changeVariableEnd == std::string::npos)
			{
				throw exceptions::DynamicPagesSyntaxException(::exceptions::variableDeclarationSyntaxError);
			}

			std::string_view variableName(code.data() + changeVariableStart, changeVariableEnd - changeVariableStart);

			code.replace(code.begin() + changeVariableStart - 1, code.begin() + changeVariableEnd + 1, findVariable(variableName));

			changeVariableStart = code.find('$');
		}

		return code;
	}

	std::vector<WFDPRenderer::ExecutionUnit> WFDPRenderer::preExecute(std::string_view code)
	{
		std::vector<ExecutionUnit> result;
		size_t startLine = 0;
		size_t endLine = code.find(';');

		if (endLine == std::string::npos)
		{
			throw exceptions::DynamicPagesSyntaxException(::exceptions::missingSemicolonSyntaxError);
		}

		result.reserve(std::count(code.begin(), code.end(), ';'));

		while (endLine != std::string::npos)
		{
			endLine++;

			std::string_view line(code.data() + startLine, endLine - startLine);
			size_t openBracket = line.find('(');
			std::string functionName(line.substr(0, openBracket));

			result.emplace_back
			(
				std::move(functionName),
				::utility::strings::split(std::string_view(line.data() + openBracket + 1, line.find(')') - openBracket - 1), argumentsDelimiter)
			);

			startLine = endLine;
			endLine = code.find(';', endLine + 1);
		}

		return result;
	}

	std::string WFDPRenderer::execute(const std::vector<ExecutionUnit>& codes)
	{
		std::string result;

		for (const auto& [functionName, arguments] : codes)
		{
			try
			{
				result += (*dynamicPagesFunctions.at(functionName))(arguments);
			}
			catch (const std::exception& e)
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

	WFDPRenderer::WFDPRenderer(const std::filesystem::path& pathToTemplates) :
		pathToTemplates(pathToTemplates)
	{
		dynamicPagesFunctions.try_emplace("print", std::make_unique<CXXDynamicFunction>(print));
		dynamicPagesFunctions.try_emplace("include", std::make_unique<CXXDynamicFunction>(std::bind(include, std::placeholders::_1, pathToTemplates.string())));
		dynamicPagesFunctions.try_emplace("for", std::make_unique<CXXDynamicFunction>(std::bind(forWFDP, std::placeholders::_1, ref(dynamicPagesFunctions))));
	}

	void WFDPRenderer::run(std::span<const interfaces::CVariable> variables, std::string& source)
	{
		size_t nextSectionStart = source.find("{%");

		while (nextSectionStart != std::string::npos)
		{
			size_t nextSectionEnd = source.find("%}", nextSectionStart);

			if (nextSectionEnd == std::string::npos)
			{
				throw exceptions::DynamicPagesSyntaxException(::exceptions::sectionDeclarationSyntaxError);
			}

			std::string code(source.begin() + nextSectionStart + 2, source.begin() + nextSectionEnd);

			WFDPRenderer::clear(code);

			WFDPRenderer::separateArguments(code);

			if (variables.size())
			{
				code = WFDPRenderer::insertVariables(variables, code);
			}

			source.replace(source.begin() + nextSectionStart, source.begin() + nextSectionEnd + 2, this->execute(WFDPRenderer::preExecute(code)));

			nextSectionStart = source.find("{%", nextSectionStart + 1);
		}

		if (source.find("{%") != std::string::npos)
		{
			this->run(variables, source);
		}
	}

	void WFDPRenderer::registerDynamicFunction(std::string_view functionName, std::unique_ptr<DynamicFunction>&& dynamicFunction)
	{
		dynamicPagesFunctions.emplace(functionName, std::move(dynamicFunction));
	}

	void WFDPRenderer::unregisterDynamicFunction(std::string_view functionName)
	{
		if (auto it = dynamicPagesFunctions.find(functionName); it != dynamicPagesFunctions.end())
		{
			dynamicPagesFunctions.erase(it);
		}
	}

	bool WFDPRenderer::isDynamicFunctionRegistered(std::string_view functionName)
	{
		return dynamicPagesFunctions.find(functionName) != dynamicPagesFunctions.end();
	}

	const std::filesystem::path& WFDPRenderer::getPathToTemplates() const
	{
		return pathToTemplates;
	}
}
