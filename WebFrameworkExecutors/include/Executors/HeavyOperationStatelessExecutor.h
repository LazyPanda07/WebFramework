#pragma once

#include "Executor.h"

namespace framework
{
	/// <summary>
	/// Base class for executors with constant or static states
	/// <para>This type of executors creating during server initializing</para>
	/// <para>Can't work in multi-threaded mode</para>
	/// <para>Run in separate thread</para>
	/// </summary>
	class EXECUTORS_API HeavyOperationStatelessExecutor : public Executor
	{
	public:
		HeavyOperationStatelessExecutor() = default;

		/// @brief Get executor type
		/// @return ExecutorType::heavyOperationStateless
		virtual utility::ExecutorType getType() const final override;

		/// @brief Stateless executors can't call destroy method
		virtual void destroy() final override;

		virtual ~HeavyOperationStatelessExecutor() = default;
	};
}
