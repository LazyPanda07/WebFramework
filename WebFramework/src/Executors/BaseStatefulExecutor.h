#pragma once

#include "BaseExecutor.h"

namespace framework
{
	class WEB_FRAMEWORK_API BaseStatefulExecutor : public BaseExecutor
	{
	public:
		BaseStatefulExecutor() = default;

		virtual BaseExecutor::executorType getType() const final;

		virtual ~BaseStatefulExecutor() = default;
	};
}