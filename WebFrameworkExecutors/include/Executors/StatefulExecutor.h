#pragma once

#include "Executor.h"

namespace framework
{
	/// <summary>
	/// Base class for executors with dynamically changes states
	/// <para>This type of executors creating dynamically for each client</para>
	/// <para>Client can change states during connection, after disconnect destroy method called</para>
	/// </summary>
	class EXECUTORS_API StatefulExecutor : public Executor
	{
	public:
		StatefulExecutor() = default;

		/// <summary>
		/// Get executor type
		/// </summary>
		/// <returns>ExecutorType::stateful</returns>
		virtual utility::ExecutorType getType() const final override;

		/// <summary>
		/// <para>Destroy and unload executor</para>
		/// <para>Executors inherited from StatelessExecutor no need this method</para>
		/// </summary>
		virtual void destroy() override;

		virtual ~StatefulExecutor() = default;
	};
}
