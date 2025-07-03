#pragma once

#include "BaseExecutor.hpp"

namespace framework
{
	class BaseStatefulExecutor : public BaseExecutor
	{
	public:
		BaseStatefulExecutor() = default;

		virtual utility::ExecutorType getType() const final override;

		virtual void destroy() override;

		virtual ~BaseStatefulExecutor() = default;
	};
}

namespace framework
{
	inline utility::ExecutorType BaseStatefulExecutor::getType() const
	{
		return utility::ExecutorType::stateful;
	}

	inline void BaseStatefulExecutor::destroy()
	{

	}
}
