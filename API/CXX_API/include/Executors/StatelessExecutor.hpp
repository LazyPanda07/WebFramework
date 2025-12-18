#pragma once

#include "Executor.hpp"

namespace framework
{
	class StatelessExecutor : public Executor
	{
	public:
		StatelessExecutor() = default;

		virtual utility::ExecutorType getType() const final override;

		virtual void destroy() final override;

		virtual ~StatelessExecutor() = default;
	};
}

namespace framework
{
	inline utility::ExecutorType StatelessExecutor::getType() const
	{
		return utility::ExecutorType::stateless;
	}

	inline void StatelessExecutor::destroy()
	{

	}
}
