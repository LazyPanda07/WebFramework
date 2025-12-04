#include "CSharpDynamicFunction.h"

#ifdef __WITH_DOT_NET_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/DotNetRuntime.h"

namespace framework
{
	CSharpDynamicFunction::CSharpDynamicFunction(char* assemblyName) :
		dynamicFunction(nullptr)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		dynamicFunction = runtime.createDynamicFunction(assemblyName);

		runtime.dealloc(assemblyName);
	}

	CSharpDynamicFunction::CSharpDynamicFunction(CSharpDynamicFunction&& other) noexcept :
		dynamicFunction(nullptr)
	{
		(*this) = std::move(other);
	}

	CSharpDynamicFunction& CSharpDynamicFunction::operator =(CSharpDynamicFunction&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		if (dynamicFunction)
		{
			runtime.free(dynamicFunction);
		}

		dynamicFunction = other.dynamicFunction;

		other.dynamicFunction = nullptr;

		return *this;
	}

	std::string CSharpDynamicFunction::operator ()(const std::vector<std::string>& arguments) const
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();
		const char** args = new const char* [arguments.size()];

		for (size_t i = 0; i < arguments.size(); i++)
		{
			args[i] = arguments[i].data();
		}

		char* resultPtr = runtime.callDynamicFunction(dynamicFunction, args, arguments.size());
		std::string result(resultPtr);

		delete[] args;
		runtime.dealloc(resultPtr);

		return result;
	}

	CSharpDynamicFunction::~CSharpDynamicFunction()
	{
		if (!dynamicFunction)
		{
			return;
		}

		runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().free(dynamicFunction);
	}
}

#endif
