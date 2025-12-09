#pragma once

#include "Executor.hpp"

namespace framework
{
	class StatefulExecutor : public Executor
	{
	public:
		StatefulExecutor() = default;

		virtual utility::ExecutorType getType() const final override;

		virtual void destroy() override;

		virtual ~StatefulExecutor() = default;
	};
}

namespace framework
{
	inline utility::ExecutorType StatefulExecutor::getType() const
	{
		return utility::ExecutorType::stateful;
	}

	inline void StatefulExecutor::destroy()
	{

	}
}
