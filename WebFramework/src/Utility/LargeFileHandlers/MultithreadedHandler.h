#pragma once

#include "BaseLargeBodyHandler.h"

#include "BaseTCPServer.h"
#include "SQLite3/SQLiteManager.h"
#include "Managers/SessionsManager.h"
#include "Managers/ExecutorsManager.h"
#include "WebNetwork/Interfaces/IStaticFile.h"
#include "WebNetwork/Interfaces/IDynamicFile.h"

//namespace framework::utility
//{
//	class MultithreadedHandler : public BaseLargeBodyHandler
//	{
//	private:
//		SessionsManager& sessionsManager;
//		const web::BaseTCPServer& serverReference;
//		interfaces::IStaticFile& staticResources;
//		interfaces::IDynamicFile& dynamicResources;
//		sqlite::SQLiteManager& database;
//		sockaddr clientAddr;
//		streams::IOSocketStream& stream;
//		ExecutorsManager& executorsManager;
//		std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors;
//		std::unique_ptr<HTTPRequest> request;
//		HTTPResponse response;
//		BaseExecutor* executor;
//		void(BaseExecutor::* method)(HTTPRequest&, HTTPResponse&);
//
//	private:
//		bool handleChunk(std::string_view data, size_t bodySize) override;
//
//		void onParseHeaders() override;
//
//		void onFinishHandleChunks() override;
//
//	public:
//		MultithreadedHandler
//		(
//			web::Network& network, SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
//			sqlite::SQLiteManager& database, sockaddr clientAddr, streams::IOSocketStream& stream, 
//			ExecutorsManager& executorsManager, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors
//		);
//
//		WaitBehavior getWaitBehavior() const override;
//
//		~MultithreadedHandler() = default;
//	};
//}
