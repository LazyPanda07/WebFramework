#include "WFDP/CXXDynamicFunction.h"

namespace framework
{
	CXXDynamicFunction::CXXDynamicFunction() :
		implementation(nullptr),
		callFunction(nullptr),
		deleter(nullptr)
	{

	}

	CXXDynamicFunction::CXXDynamicFunction(const std::function<std::string(const std::vector<std::string>&)>& functor) :
		functor(functor),
		implementation(nullptr),
		callFunction(nullptr),
		deleter(nullptr)
	{
		
	}

	void CXXDynamicFunction::initClass(void* data)
	{
		struct DynamicFunctionController
		{
			void* dynamicFunction;
			void (*callFunction)(void* dynamicFunction, const std::span<std::string_view>& arguments, void* data, void(*callback)(const char* result, size_t size, void* data));
			void (*deleter)(void* implementation);
		};

		const DynamicFunctionController* controller = static_cast<const DynamicFunctionController*>(data);

		implementation = controller->dynamicFunction;
		callFunction = controller->callFunction;
		deleter = controller->deleter;
	}

	std::string CXXDynamicFunction::operator()(const std::vector<std::string>& arguments) const
	{
		if (callFunction)
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

		return functor(arguments);
	}

	CXXDynamicFunction::~CXXDynamicFunction()
	{
		if (deleter)
		{
			deleter(implementation);
		}
	}
}
