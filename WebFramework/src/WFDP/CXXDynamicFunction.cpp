#include "WFDP/CXXDynamicFunction.h"

namespace framework
{
	CXXDynamicFunction::DynamicFunctionController::DynamicFunctionController() :
		dynamicFunction(nullptr),
		callFunction(nullptr),
		deleter(nullptr)
	{

	}

	CXXDynamicFunction::CXXDynamicFunction(const void* data) :
		implementation(nullptr),
		callFunction(nullptr),
		deleter(nullptr)
	{
		const DynamicFunctionController* controller = static_cast<const DynamicFunctionController*>(data);

		implementation = controller->dynamicFunction;
		callFunction = controller->callFunction;
		deleter = controller->deleter;
	}

	std::string CXXDynamicFunction::operator()(const std::vector<std::string>& arguments) const
	{
		auto getResult = [](const char* result, size_t size, void* data)
			{
				static_cast<std::string*>(data)->append(result, size);
			};

		std::string result;
		std::vector<std::string_view> views;

		views.reserve(arguments.size());

		for (const std::string& argument : arguments)
		{
			views.emplace_back(argument);
		}

		callFunction(implementation, views, &result, getResult);

		return result;
	}

	CXXDynamicFunction::~CXXDynamicFunction()
	{
		if (deleter)
		{
			deleter(implementation);
		}
	}
}
