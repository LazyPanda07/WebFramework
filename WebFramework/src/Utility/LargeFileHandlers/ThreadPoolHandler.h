#pragma once

#include "BaseLargeBodyHandler.h"

namespace framework::utility
{
	class ThreadPoolHandler : public BaseLargeBodyHandler
	{
	public:
		ThreadPoolHandler(web::Network& network);

		WaitBehavior getWaitBehavior() const override;

		~ThreadPoolHandler() = default;
	};
}
