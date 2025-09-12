#pragma once

#include "BaseExecutor.hpp"

namespace framework
{
	class BaseHeavyOperationStatefulExecutor : public BaseExecutor
	{
	public:
		BaseHeavyOperationStatefulExecutor() = default;

		virtual utility::ExecutorType getType() const final override;

		virtual void destroy() override;

		virtual ~BaseHeavyOperationStatefulExecutor() = default;
	};
}

namespace framework
{
	inline utility::ExecutorType BaseHeavyOperationStatefulExecutor::getType() const
	{
		return utility::ExecutorType::heavyOperationStateful;
	}

	inline void BaseHeavyOperationStatefulExecutor::destroy()
	{

	}
}
