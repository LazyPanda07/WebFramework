#pragma once

#include "BaseExecutor.hpp"

namespace framework
{
	class BaseHeavyOperationStatelessExecutor : public BaseExecutor
	{
	public:
		BaseHeavyOperationStatelessExecutor() = default;

		virtual utility::ExecutorType getType() const final override;

		virtual void destroy() final override;

		virtual ~BaseHeavyOperationStatelessExecutor() = default;
	};
}

namespace framework
{
	inline utility::ExecutorType BaseHeavyOperationStatelessExecutor::getType() const
	{
		return utility::ExecutorType::heavyOperationStateless;
	}

	inline void BaseHeavyOperationStatelessExecutor::destroy()
	{

	}
}
