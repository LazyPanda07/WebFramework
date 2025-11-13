#pragma once

#include "BaseExecutor.hpp"

namespace framework
{
	class BaseStatelessExecutor : public BaseExecutor
	{
	public:
		BaseStatelessExecutor() = default;

		virtual utility::ExecutorType getType() const final override;

		virtual void destroy() final override;

		virtual ~BaseStatelessExecutor() = default;
	};
}

namespace framework
{
	inline utility::ExecutorType BaseStatelessExecutor::getType() const
	{
		return utility::ExecutorType::stateless;
	}

	inline void BaseStatelessExecutor::destroy()
	{

	}
}
