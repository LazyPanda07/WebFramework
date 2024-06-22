#pragma once

#include "BaseExecutor.h"

namespace framework
{
	/// <summary>
	/// Base class for executors with dynamically changes states
	/// <para>This type of executors creating dynamically for each client</para>
	/// <para>Client can change states during connection, after disconnect destroy method called</para>
	/// <para>Run in separate thread</para>
	/// </summary>
	class WEB_FRAMEWORK_API BaseHeavyOperationStatefulExecutor : public BaseExecutor
	{
	public:
		BaseHeavyOperationStatefulExecutor() = default;

		/// @brief Get executor type
		/// @return executorType::heavyOperationStateful
		virtual executorType getType() const final override;

		virtual void destroy() override;

		virtual ~BaseHeavyOperationStatefulExecutor() = default;
	};
}
