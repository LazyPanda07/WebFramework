#pragma once

#include "Executor.hpp"

namespace framework
{
	class HeavyOperationStatefulExecutor : public Executor
	{
	public:
		HeavyOperationStatefulExecutor() = default;

		virtual utility::ExecutorType getType() const final override;

		virtual void destroy() override;

		virtual ~HeavyOperationStatefulExecutor() = default;
	};
}

namespace framework
{
	inline utility::ExecutorType HeavyOperationStatefulExecutor::getType() const
	{
		return utility::ExecutorType::heavyOperationStateful;
	}

	inline void HeavyOperationStatefulExecutor::destroy()
	{

	}
}
