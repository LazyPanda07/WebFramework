#include "WFDP/CSharpDynamicFunction.h"

#ifdef __WITH_DOTNET_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/DotNetRuntime.h"

namespace framework
{
	CSharpDynamicFunction::CSharpDynamicFunction(char* assemblyName) :
		dynamicFunction(nullptr)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		dynamicFunction = runtime.getCreateDynamicFunction()(assemblyName);

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

	std::string CSharpDynamicFunction::operator ()(const json::JsonObject& arguments) const
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();
		char* resultPtr = runtime.getCallDynamicFunction()(dynamicFunction, &arguments);
		std::string result(resultPtr);

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
