#include "WFDP/StandardWebFrameworkDynamicPagesFunctions.h"

#include <fstream>

#include <Exceptions/FileDoesNotExistException.h>

#include "WFDP/CXXDynamicFunction.h"

class StandardFunction
{
protected:
	static void callFunction(void* dynamicFunction, const std::span<std::string_view>& arguments, void* data, void(*callback)(const char* result, size_t size, void* data));

public:
	StandardFunction() = default;

	virtual std::string call(const std::span<std::string_view>& arguments) = 0;

	virtual ~StandardFunction() = default;
};

class PrintFunction : public StandardFunction
{
private:
	PrintFunction() = default;

public:
	static std::unique_ptr<framework::DynamicFunction> create();

public:
	std::string call(const std::span<std::string_view>& arguments) override;
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
	std::string call(const std::span<std::string_view>& arguments) override;
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
	std::string call(const std::span<std::string_view>& arguments) override;
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

void StandardFunction::callFunction(void* dynamicFunction, const std::span<std::string_view>& arguments, void* data, void(*callback)(const char* result, size_t size, void* data))
{
	std::string result = static_cast<StandardFunction*>(dynamicFunction)->call(arguments);

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

std::string PrintFunction::call(const std::span<std::string_view>& arguments)
{
	std::string result;

	for (std::string_view data : arguments)
	{
		result += std::format("{} ", data);
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

std::string IncludeFunction::call(const std::span<std::string_view>& arguments)
{
	const std::filesystem::path filePath(pathToTemplates / arguments[0]);

	if (!std::filesystem::exists(filePath))
	{
		throw file_manager::exceptions::FileDoesNotExistException(filePath);
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

std::string ForFunction::call(const std::span<std::string_view>& arguments)
{
	int64_t start = std::stoll(arguments[0].data());
	int64_t end = std::stoll(arguments[1].data());
	framework::DynamicFunction& repeatableFunction = *dynamicPagesFunctions.at(arguments[2].data());
	int64_t step = 1;
	std::string result;

	if (arguments.size() == 4)
	{
		step = std::stoll(arguments[3].data());
	}

	for (int64_t i = start; i < end; i += step)
	{
		result += repeatableFunction({ std::to_string(i) });
	}

	return result;
}
