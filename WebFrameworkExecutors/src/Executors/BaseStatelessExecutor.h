#pragma once

#include "BaseExecutor.h"

namespace framework
{
	/// <summary>
	/// Base class for executors with constant or static states
	/// <para>This type of executors creating during server initializing</para>
	/// <para>Can't work in multi-threaded mode</para>
	/// </summary>
	class EXECUTORS_API BaseStatelessExecutor : public BaseExecutor
	{
	public:
		BaseStatelessExecutor() = default;

		/// <summary>
		/// Get executor type
		/// </summary>
		/// <returns>ExecutorType::stateless</returns>
		virtual utility::ExecutorType getType() const final override;

		/// <summary>
		/// Stateless executors can't call destroy method
		/// </summary>
		virtual void destroy() final override;

		virtual ~BaseStatelessExecutor() = default;
	};
}
