#pragma once

#include "BaseExecutor.h"

namespace framework
{
	class BaseHeavyOperationStatefullExecutor : public BaseExecutor
	{
	public:
		BaseHeavyOperationStatefullExecutor() = default;

		/// @brief Get executor type
		/// @return executorType::heavyOperationStateful
		virtual BaseExecutor::executorType getType() const final override;

		virtual ~BaseHeavyOperationStatefullExecutor() = default;
	};
}
