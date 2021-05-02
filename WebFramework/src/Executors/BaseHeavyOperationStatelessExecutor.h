#pragma once

#include "BaseExecutor.h"

namespace framework
{
	class BaseHeavyOperationStatelessExecutor : public BaseExecutor
	{
	public:
		BaseHeavyOperationStatelessExecutor() = default;

		/// @brief Get executor type
		/// @return executorType::heavyOperationStateless
		virtual BaseExecutor::executorType getType() const final override;

		/// @brief Stateless executors can't call destroy method
		virtual void destroy() final override;

		virtual ~BaseHeavyOperationStatelessExecutor() = default;
	};
}
