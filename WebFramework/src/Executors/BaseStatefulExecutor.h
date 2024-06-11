#pragma once

#include "BaseExecutor.h"

namespace framework
{
	/// <summary>
	/// Base class for executors with dynamically changes states
	/// <para>This type of executors creating dynamically for each client</para>
	/// <para>Client can change states during connection, after disconnect destroy method called</para>
	/// </summary>
	class BaseStatefulExecutor : public BaseExecutor
	{
	public:
		BaseStatefulExecutor() = default;

		/// <summary>
		/// Get executor type
		/// </summary>
		/// <returns>executorType::stateful</returns>
		virtual executorType getType() const final override;

		virtual void destroy() override;

		virtual ~BaseStatefulExecutor() = default;
	};
}
