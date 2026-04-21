#pragma once

#include "LargeBodyHandler.h"

#include "BaseTCPServer.h"
#include "Managers/SessionsManager.h"
#include "Managers/ExecutorsManager.h"
#include "WebInterfaces/IStaticFile.h"
#include "WebInterfaces/IDynamicFile.h"
#include "Web/HttpRequestImplementation.h"

namespace framework::utility
{
	class BaseLargeBodyHandler : public web::LargeBodyHandler
	{
	private:
		SessionsManager& sessionsManager;
		BaseWebServer& serverReference;
		interfaces::IStaticFile& staticResources;
		interfaces::IDynamicFile& dynamicResources;
		sockaddr clientAddr;
		streams::IOSocketStream& stream;
		ExecutorsManager& executorsManager;
		ExecutorsManager::StatefulExecutors& executors;
		std::unique_ptr<HttpRequestImplementation> request;
		HttpResponseImplementation response;
		Executor* executor;
		void(Executor::* method)(interfaces::IHttpRequest&, interfaces::IHttpResponse&);

	private:
		bool handleChunk(std::string_view data) override;

		void onParseHeaders() override;

		void onFinishHandleChunks() override;

	public:
		BaseLargeBodyHandler
		(
			web::Network& network, SessionsManager& session, BaseWebServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
			sockaddr clientAddr, streams::IOSocketStream& stream,
			ExecutorsManager& executorsManager, ExecutorsManager::StatefulExecutors& executors
		);

		~BaseLargeBodyHandler() = default;
	};
}
