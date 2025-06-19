#pragma once

#include <Executors/BaseExecutor.h>

#include <Utility/Sources.h>

namespace framework
{
	class CXXExecutor : public BaseExecutor
	{
	public:
		using GetTypeSignature = ExecutorType(*)(void*);
		using DestroySignature = void(*)(void*);

	private:
		void* implementation;
		GetTypeSignature getTypeFunction;
		DestroySignature destroyFunction;

	public:
		CXXExecutor(HMODULE module, void* implementation);

		ExecutorType getType() const override;

		void destroy() override;
	};
}
