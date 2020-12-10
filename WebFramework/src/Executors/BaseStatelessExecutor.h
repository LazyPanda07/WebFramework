#pragma once

#include "BaseExecutor.h"

namespace framework
{
	class WEB_FRAMEWORK_API BaseStatelessExecutor : public BaseExecutor
	{
	public:
		BaseStatelessExecutor() = default;

		virtual BaseExecutor::executorType getType() const final;

		virtual ~BaseStatelessExecutor() = default;
	};
}