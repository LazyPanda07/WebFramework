#pragma once

#include "DynamicFunction.h"

#ifdef __WITH_DOTNET_EXECUTORS__

namespace framework
{
	class CSharpDynamicFunction : public DynamicFunction
	{
	private:
		void* dynamicFunction;

	public:
		CSharpDynamicFunction(char* assemblyName);

		CSharpDynamicFunction(CSharpDynamicFunction&) = delete;

		CSharpDynamicFunction(CSharpDynamicFunction&& other) noexcept;

		CSharpDynamicFunction& operator =(CSharpDynamicFunction&) = delete;

		CSharpDynamicFunction& operator =(CSharpDynamicFunction&& other) noexcept;

		std::string operator ()(const json::JsonObject& arguments) const override;

		~CSharpDynamicFunction();
	};
}

#endif
