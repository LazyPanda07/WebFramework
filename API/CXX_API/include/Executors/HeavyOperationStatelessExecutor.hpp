#pragma once

#include "Executor.hpp"

namespace framework
{
	class HeavyOperationStatelessExecutor : public Executor
	{
	public:
		HeavyOperationStatelessExecutor() = default;

		virtual utility::ExecutorType getType() const final override;

		virtual void destroy() final override;

		virtual ~HeavyOperationStatelessExecutor() = default;
	};
}

namespace framework
{
	inline utility::ExecutorType HeavyOperationStatelessExecutor::getType() const
	{
		return utility::ExecutorType::heavyOperationStateless;
	}

	inline void HeavyOperationStatelessExecutor::destroy()
	{

	}
}
