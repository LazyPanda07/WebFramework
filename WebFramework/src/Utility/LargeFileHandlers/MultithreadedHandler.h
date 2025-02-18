#pragma once

#include "BaseLargeBodyHandler.h"

namespace framework::utility
{
	class MultithreadedHandler : public BaseLargeBodyHandler
	{
	public:
		MultithreadedHandler(web::Network& network);

		WaitBehavior getWaitBehavior() const override;

		~MultithreadedHandler() = default;
	};
}
