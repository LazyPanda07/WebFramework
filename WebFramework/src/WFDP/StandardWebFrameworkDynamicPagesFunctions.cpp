#include "WFDP/StandardWebFrameworkDynamicPagesFunctions.h"

#include <fstream>
#include <format>
#include <sstream>

#include <MapJsonIterator.h>

#include "WFDP/CXXDynamicFunction.h"
#include "Utility/Utils.h"

class StandardFunction
{
protected:
	static void callFunction(void* dynamicFunction, const void* arguments, void* data, void(*callback)(const char* result, size_t size, void* data));

public:
	StandardFunction() = default;

	virtual std::string call(const json::JsonObject& arguments) = 0;

	virtual ~StandardFunction() = default;
};

class PrintFunction : public StandardFunction
{
private:
	PrintFunction() = default;

public:
	static std::unique_ptr<framework::DynamicFunction> create();

public:
	std::string call(const json::JsonObject& arguments) override;
};

class IncludeFunction : public StandardFunction
{
private:
	std::filesystem::path pathToTemplates;

private:
	IncludeFunction(const std::filesystem::path& pathToTemplates);

public:
	static std::unique_ptr<framework::DynamicFunction> create(const std::filesystem::path& pathToTemplates);

public:
	std::string call(const json::JsonObject& arguments) override;
};

class ForFunction : public StandardFunction
{
private:
	const ::utility::strings::string_based_unordered_map<std::unique_ptr<framework::DynamicFunction>>& dynamicPagesFunctions;

private:
	ForFunction(const ::utility::strings::string_based_unordered_map<std::unique_ptr<framework::DynamicFunction>>& dynamicPagesFunctions);

public:
	static std::unique_ptr<framework::DynamicFunction> create(const ::utility::strings::string_based_unordered_map<std::unique_ptr<framework::DynamicFunction>>& dynamicPagesFunctions);

public:
	std::string call(const json::JsonObject& arguments) override;
};

namespace framework
{
	std::unique_ptr<DynamicFunction> createPrintFunction()
	{
		return PrintFunction::create();
	}

	std::unique_ptr<DynamicFunction> createIncludeFunction(const std::filesystem::path& pathToTemplates)
	{
		return IncludeFunction::create(pathToTemplates);
	}

	std::unique_ptr<DynamicFunction> createForFunction(const ::utility::strings::string_based_unordered_map<std::unique_ptr<DynamicFunction>>& dynamicPagesFunctions)
	{
		return ForFunction::create(dynamicPagesFunctions);
	}
}

void StandardFunction::callFunction(void* dynamicFunction, const void* arguments, void* data, void(*callback)(const char* result, size_t size, void* data))
{
	std::string result = static_cast<StandardFunction*>(dynamicFunction)->call(*static_cast<const json::JsonObject*>(arguments));

	callback(result.data(), result.size(), data);
}

std::unique_ptr<framework::DynamicFunction> PrintFunction::create()
{
	framework::CXXDynamicFunction::DynamicFunctionController controller;

	controller.dynamicFunction = new PrintFunction();
	controller.deleter = [](void* implementation) { delete static_cast<PrintFunction*>(implementation); };
	controller.callFunction = &StandardFunction::callFunction;

	return std::make_unique<framework::CXXDynamicFunction>(&controller);
}

std::string PrintFunction::call(const json::JsonObject& arguments)
{
	const json::JsonObject& object = arguments["@print"];
	std::string printType = "default";
	std::string result;

	object.tryGet<std::string>("type", printType);

	if (printType == "json")
	{
		return static_cast<std::string>(object);
	}

	json::MapJsonIterator it(object);

	for (const auto& [_, value] : it)
	{
		switch (value.getEnumType())
		{
		case json::utility::JsonVariantTypeEnum::jNull:
			result += "null ";

			break;

		case json::utility::JsonVariantTypeEnum::jString:
			result += value.get<std::string>() + ' ';

			break;

		case json::utility::JsonVariantTypeEnum::jBool:
			result += value.get<bool>() ? "true " : "false ";

			break;

		case json::utility::JsonVariantTypeEnum::jInt:
			result += std::to_string(value.get<int64_t>()) + ' ';

			break;

		case json::utility::JsonVariantTypeEnum::jUint:
			result += std::to_string(value.get<uint64_t>()) + ' ';

			break;

		case json::utility::JsonVariantTypeEnum::jDouble:
			result += std::to_string(value.get<double>()) + ' ';

			break;

		case json::utility::JsonVariantTypeEnum::jJSONArray:
		case json::utility::JsonVariantTypeEnum::jJSONObject:
			result += static_cast<std::string>(value) + ' ';

			break;

		default:
			break;
		}
	}

	result.pop_back();

	return result;
}

IncludeFunction::IncludeFunction(const std::filesystem::path& pathToTemplates) :
	pathToTemplates(pathToTemplates)
{

}

std::unique_ptr<framework::DynamicFunction> IncludeFunction::create(const std::filesystem::path& pathToTemplates)
{
	framework::CXXDynamicFunction::DynamicFunctionController controller;

	controller.dynamicFunction = new IncludeFunction(pathToTemplates);
	controller.deleter = [](void* implementation) { delete static_cast<IncludeFunction*>(implementation); };
	controller.callFunction = &StandardFunction::callFunction;

	return std::make_unique<framework::CXXDynamicFunction>(&controller);
}

std::string IncludeFunction::call(const json::JsonObject& arguments)
{
	const std::filesystem::path filePath(pathToTemplates / arguments["@include"]["includePath"].get<std::string>());

	if (!std::filesystem::exists(filePath))
	{
		framework::utility::logAndThrowException<framework::logging::message::cantFindAssetFromIncludePath, framework::logging::category::dynamicFunction>(filePath.string());
	}

	return (std::ostringstream() << std::ifstream(filePath).rdbuf()).str();
}

ForFunction::ForFunction(const ::utility::strings::string_based_unordered_map<std::unique_ptr<framework::DynamicFunction>>& dynamicPagesFunctions) :
	dynamicPagesFunctions(dynamicPagesFunctions)
{

}

std::unique_ptr<framework::DynamicFunction> ForFunction::create(const ::utility::strings::string_based_unordered_map<std::unique_ptr<framework::DynamicFunction>>& dynamicPagesFunctions)
{
	framework::CXXDynamicFunction::DynamicFunctionController controller;

	controller.dynamicFunction = new ForFunction(dynamicPagesFunctions);
	controller.deleter = [](void* implementation) { delete static_cast<ForFunction*>(implementation); };
	controller.callFunction = &StandardFunction::callFunction;

	return std::make_unique<framework::CXXDynamicFunction>(&controller);
}

std::string ForFunction::call(const json::JsonObject& arguments)
{
	const json::JsonObject& forField = arguments["@for"].get<json::JsonObject>();

	int64_t start = forField["start"].get<int64_t>();
	int64_t end = forField["end"].get<int64_t>();
	const std::string& functionName = forField["functionName"].get<std::string>();
	framework::DynamicFunction& repeatableFunction = *dynamicPagesFunctions.at(functionName);
	int64_t step = 1;
	std::string result;

	if (forField.contains<int64_t>("step"))
	{
		step = forField["step"].get<int64_t>();
	}

	json::JsonObject wrapper;
	json::JsonObject& repeatableFunctionData = wrapper["@" + functionName];

	for (int64_t i = start; i < end; i += step)
	{
		repeatableFunctionData["currentIndex"] = i;

		result += repeatableFunction(wrapper);
	}

	return result;
}
