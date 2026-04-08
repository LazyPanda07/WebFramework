#include "Rendering/WFDPRenderer.h"

#include <PatternParser.h>
#include <MapJsonIterator.h>

#include "Exceptions/DynamicPagesSyntaxException.h"
#include "WFDP/StandardWebFrameworkDynamicPagesFunctions.h"
#include "WFDP/CXXDynamicFunction.h"
#include "Framework/WebFrameworkConstants.h"
#include "ExecutorsConstants.h"
#include "Utility/Utils.h"

#ifndef __LINUX__
#pragma warning(disable: 26800)
#endif

template<>
struct utility::parsers::Converter<std::string>
{
	constexpr void convert(std::string_view data, std::string& result)
	{
		result = data;
	}
};

namespace framework
{
	WFDPRenderer::ExecutionUnit::ExecutionUnit(std::string_view functionName, const json::JsonObject& sharedArguments) :
		functionName(functionName),
		sharedArguments(sharedArguments)
	{

	}

	std::vector<WFDPRenderer::ExecutionUnit> WFDPRenderer::parse(std::string_view code, const json::JsonObject& sharedArguments, bool checks)
	{
		auto getOffset = [](std::string_view data) -> size_t
			{
				for (size_t i = 0; i < data.size(); i++)
				{
					if ((data[i] >= 'A' && data[i] <= 'Z') || (data[i] >= 'a' && data[i] <= 'z'))
					{
						return i;
					}
				}

				utility::logAndThrowException<logging::message::notValidFunctionName, logging::category::dynamicFunction>();
			};

		constexpr ::utility::parsers::PatternParser<std::string, std::string> argumentParser("{}: {}");
		constexpr ::utility::parsers::PatternParser<std::string, std::string, std::string> argumentParserWithDefaultValue("{}: {} = {}");
		size_t startLine = 0;
		size_t endLine = code.find(';');
		std::vector<ExecutionUnit> result;

		if (endLine == std::string_view::npos)
		{
			utility::logAndThrowException<logging::message::missingSemicolonSyntaxError, logging::category::dynamicFunction>(code);
		}

		while (endLine != std::string_view::npos)
		{
			endLine++;

			std::string_view line(code.data() + startLine, endLine - startLine);
			size_t openBracket = line.find('(');
			size_t closeBracket = line.find(')');

			if (openBracket == std::string_view::npos)
			{
				utility::logAndThrowException<logging::message::missingOpenBracketSyntaxError, logging::category::dynamicFunction>(line);
			}

			if (closeBracket == std::string_view::npos)
			{
				utility::logAndThrowException<logging::message::missingCloseBracketSyntaxError, logging::category::dynamicFunction>(line);
			}

			ExecutionUnit& executionUnit = result.emplace_back
			(
				std::string(line.begin() + getOffset(line), line.begin() + openBracket),
				sharedArguments
			);
			std::string functionFieldName = "@" + executionUnit.functionName;
			size_t argumentOffset = openBracket + 1;

			if (line[argumentOffset] != ')')
			{
				do
				{
					size_t comma = line.find(',', argumentOffset);

					std::string_view argument(line.begin() + argumentOffset, line.begin() + (std::min)(comma, closeBracket));
					std::string name;
					std::string type;
					std::string defaultValue;

					if (argument.find('=') != std::string_view::npos)
					{
						argumentParserWithDefaultValue.parse(argument, name, type, defaultValue);
					}
					else
					{
						argumentParser.parse(argument, name, type);
					}

					if (defaultValue.size() || type == "null")
					{
						json::JsonObject value;

						try
						{
							if (type == "int")
							{
								value = std::stoll(defaultValue);
							}
							else if (type == "double")
							{
								value = std::stod(defaultValue);
							}
							else if (type == "bool")
							{
								if (defaultValue == "true" || defaultValue == "false")
								{
									value = defaultValue == "true" ? true : false;
								}
								else
								{
									utility::logAndThrowException<logging::message::wrongDefaultValueForType, logging::category::dynamicFunction>(defaultValue, type);
								}
							}
							else if (type == "string")
							{
								constexpr ::utility::parsers::PatternParser<std::string> stringParser(R"("{}")");

								std::string temp;

								stringParser.parse(defaultValue, temp);

								value = std::move(temp);
							}
							else if (type == "null")
							{
								value = nullptr;
							}
							else
							{
								utility::logAndThrowException<logging::message::defaultValueForTypeDoesNotSupported, logging::category::dynamicFunction>(type);
							}
						}
						catch (const std::invalid_argument&)
						{
							utility::logAndThrowException<logging::message::cantConvertDefaultValue, logging::category::dynamicFunction>(defaultValue, type);
						}
						catch (const std::out_of_range&)
						{
							utility::logAndThrowException<logging::message::outOfRangeDefaultValue, logging::category::dynamicFunction>(defaultValue, type);
						}

						executionUnit.defaultArguments[std::move(name)] = std::move(value);
					}
					else if (checks)
					{
						constexpr std::array<std::string_view, 7> types =
						{
							"int",
							"double",
							"string",
							"bool",
							"array",
							"object",
							"null"
						};

						if (!sharedArguments.contains<json::JsonObject>(functionFieldName))
						{
							utility::logAndThrowException<logging::message::cantFindFunctionField, logging::category::dynamicFunction>(executionUnit.functionName, executionUnit.functionName);
						}

						if (!sharedArguments[functionFieldName].contains(name))
						{
							utility::logAndThrowException<logging::message::cantFindArgument, logging::category::dynamicFunction>(name, executionUnit.functionName);
						}

						if (std::ranges::find(types, type) == types.end())
						{
							utility::logAndThrowException<logging::message::wrongType, logging::category::dynamicFunction>(type, executionUnit.functionName);
						}
					}

					argumentOffset += argument.size();

					if (comma == std::string_view::npos)
					{
						break;
					}

					do
					{
						if (std::isspace(line[argumentOffset]) || line[argumentOffset] == ',')
						{
							argumentOffset++;

							continue;
						}

						break;
					}
					while (true);
				}
				while (true);
			}

			startLine = endLine;
			endLine = code.find(';', endLine + 1);
		}

		return result;
	}

	std::string WFDPRenderer::execute(const std::vector<ExecutionUnit>& codes)
	{
		std::string result;

		for (const auto& [functionName, arguments, defaultArguments] : codes)
		{
			try
			{
				if (Log::isValid())
				{
					Log::info<logging::message::callDynamicFunction, logging::category::dynamicFunction>(functionName);
				}

				if (defaultArguments.size() != (std::numeric_limits<size_t>::max)())
				{
					json::JsonObject temp(arguments);
					json::MapJsonIterator it(defaultArguments);
					json::JsonObject& functionParameters = temp["@" + functionName];

					for (const auto& [key, value] : it)
					{
						functionParameters[key] = value;
					}

					result += (*dynamicPagesFunctions.at(functionName))(temp);
				}
				else
				{
					result += (*dynamicPagesFunctions.at(functionName))(arguments);
				}
			}
			catch (const std::exception& e)
			{
				utility::logAndThrowException<logging::message::wfdpRendererExecuteException, logging::category::dynamicFunction>(e.what());
			}
		}

		return result;
	}

	WFDPRenderer::WFDPRenderer(const std::filesystem::path& pathToTemplates) :
		pathToTemplates(pathToTemplates)
	{
		dynamicPagesFunctions.try_emplace("print", createPrintFunction());
		dynamicPagesFunctions.try_emplace("include", createIncludeFunction(pathToTemplates));
		dynamicPagesFunctions.try_emplace("for", createForFunction(dynamicPagesFunctions));
	}

	void WFDPRenderer::run(const void* arguments, std::string& source)
	{
		size_t nextSectionStart = source.find("{%");
		const json::JsonObject& sharedArguments = *static_cast<const json::JsonObject*>(arguments);

		while (nextSectionStart != std::string::npos)
		{
			size_t nextSectionEnd = source.find("%}", nextSectionStart);

			if (nextSectionEnd == std::string::npos)
			{
				utility::logAndThrowException<logging::message::sectionDeclarationSyntaxError, logging::category::dynamicFunction>(source);
			}

			std::string_view code(source.begin() + nextSectionStart + 2, source.begin() + nextSectionEnd);

			source.replace
			(
				source.begin() + nextSectionStart,
				source.begin() + nextSectionEnd + 2,
				this->execute(WFDPRenderer::parse(code, sharedArguments, true))
			);

			nextSectionStart = source.find("{%", nextSectionStart + 1);
		}

		if (source.find("{%") != std::string::npos)
		{
			this->run(arguments, source);
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
