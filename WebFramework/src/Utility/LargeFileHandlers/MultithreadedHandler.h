#pragma once

#include "BaseLargeBodyHandler.h"

namespace framework::utility
{
	class MultithreadedHandler : public BaseLargeBodyHandler
	{
	public:
		MultithreadedHandler
		(
			web::Network& network, SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
			sockaddr clientAddr, streams::IOSocketStream& stream, 
			ExecutorsManager& executorsManager, ExecutorsManager::StatefulExecutors& executors
		);

		WaitBehavior getWaitBehavior() const override;

		~MultithreadedHandler() = default;
	};
}
