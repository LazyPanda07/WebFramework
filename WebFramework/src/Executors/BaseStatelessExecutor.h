#pragma once

#include "BaseExecutor.h"

namespace framework
{
	/// <summary>
	/// <para>Base class for executors with constant or static states</para>
	/// <para>This type of executors creating during server initializing</para>
	/// <para>Can't work in multi-threaded mode</para>
	/// </summary>
	class WEB_FRAMEWORK_API BaseStatelessExecutor : public BaseExecutor
	{
	public:
		BaseStatelessExecutor() = default;

		/// <summary>
		/// Get executor type
		/// </summary>
		/// <returns>executorType::stateless</returns>
		virtual BaseExecutor::executorType getType() const final;

		/// <summary>
		/// Stateless executors can't call destroy method
		/// </summary>
		virtual void destroy() override final;

		virtual ~BaseStatelessExecutor() = default;
	};
}
