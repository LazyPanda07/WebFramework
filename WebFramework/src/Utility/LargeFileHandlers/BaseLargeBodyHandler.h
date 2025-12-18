#pragma once

#include "LargeBodyHandler.h"

#include "BaseTCPServer.h"
#include "Managers/SessionsManager.h"
#include "Managers/ExecutorsManager.h"
#include "WebInterfaces/IStaticFile.h"
#include "WebInterfaces/IDynamicFile.h"
#include "Web/HTTPRequestImplementation.h"

namespace framework::utility
{
	class BaseLargeBodyHandler : public web::LargeBodyHandler
	{
	private:
		SessionsManager& sessionsManager;
		const web::BaseTCPServer& serverReference;
		interfaces::IStaticFile& staticResources;
		interfaces::IDynamicFile& dynamicResources;
		sockaddr clientAddr;
		streams::IOSocketStream& stream;
		ExecutorsManager& executorsManager;
		ExecutorsManager::StatefulExecutors& executors;
		std::unique_ptr<HTTPRequestImplementation> request;
		HTTPResponseImplementation response;
		std::unique_ptr<HTTPRequestExecutors> requestWrapper;
		HTTPResponseExecutors responseWrapper;
		BaseExecutor* executor;
		void(BaseExecutor::* method)(HTTPRequestExecutors&, HTTPResponseExecutors&);

	private:
		bool handleChunk(std::string_view data) override;

		void onParseHeaders() override;

		void onFinishHandleChunks() override;

	public:
		BaseLargeBodyHandler
		(
			web::Network& network, SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
			sockaddr clientAddr, streams::IOSocketStream& stream,
			ExecutorsManager& executorsManager, ExecutorsManager::StatefulExecutors& executors
		);

		~BaseLargeBodyHandler() = default;
	};
}
