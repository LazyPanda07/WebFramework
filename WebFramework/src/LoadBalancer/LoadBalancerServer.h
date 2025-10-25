#pragma once

#include "Web/Servers/BaseWebServer.h"

#include <chrono>

#include <MultiLocalizationManager.h>
#include <IOSocketStream.h>

#include "Framework/WebFrameworkPlatform.h"
#include "Heuristics/BaseLoadBalancerHeuristic.h"
#include "Executors/ResourceExecutor.h"

#include "Utility/BaseConnectionData.h"
#include "Utility/AdditionalServerSettings.h"
#include "Utility/ConcurrentQueue.h"
#include "Utility/Sources.h"

namespace framework::load_balancer
{
	class LoadBalancerServer : public BaseWebServer
	{
	private:
		struct ServerData
		{
		public:
			utility::BaseConnectionData connectionData;
			std::unique_ptr<BaseLoadBalancerHeuristic> heuristic;

		public:
			ServerData(utility::BaseConnectionData&& connectionData, std::unique_ptr<BaseLoadBalancerHeuristic>&& heuristic) noexcept;

			ServerData(const ServerData&) = delete;

			ServerData(ServerData&&) noexcept = default;

			ServerData& operator = (const ServerData&) = delete;

			ServerData& operator = (ServerData&&) noexcept = default;

			~ServerData() = default;
		};

		struct LoadBalancerRequest
		{
		public:
			enum class State
			{
				receiveClientRequest,
				receiveServerResponse
			};

		public:
			streams::IOSocketStream clientStream;
			streams::IOSocketStream serverStream;
			std::function<void()> cleanup;
			BaseLoadBalancerHeuristic* heuristic;
			State currentState;

		public:
			LoadBalancerRequest(streams::IOSocketStream&& clientStream, streams::IOSocketStream&& serverStream, std::unique_ptr<BaseLoadBalancerHeuristic>& heuristic, std::function<void()>&& cleanup);

			LoadBalancerRequest(LoadBalancerRequest&& other) noexcept = default;

			LoadBalancerRequest& operator =(LoadBalancerRequest&& other) noexcept = default;

			~LoadBalancerRequest();
		};

	private:
		std::vector<ServerData> allServers;
		std::vector<std::vector<LoadBalancerRequest>> requestQueues;
		std::vector<std::future<void>> threads;
		threading::utility::ConcurrentQueue<LoadBalancerRequest> queuedRequests;
		std::shared_ptr<ResourceExecutor> resources;
		std::chrono::microseconds threshold;
		bool serversHTTPS;

	private:
		static bool receiveClientRequest(LoadBalancerRequest& request, std::string& httpRequest);

		static bool sendClientRequest(LoadBalancerRequest& request, std::string& httpRequest);

		static bool receiveServerResponse(LoadBalancerRequest& request, std::string& httpResponse);

		static bool sendClientResponse(LoadBalancerRequest& request, const std::string& httpResponse);

	private:
		void processing(size_t index);

		std::unique_ptr<BaseLoadBalancerHeuristic> createAPIHeuristic(std::string_view ip, std::string_view port, bool useHTTPS, std::string_view heuristicName, std::string_view apiType, utility::LoadSource loadSource) const;

	private:
		void receiveConnections(const std::function<void()>& onStartServer, std::exception** outException) override;

		void clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr addr, std::function<void()>& cleanup) override;

	public:
		LoadBalancerServer
		(
			std::string_view ip, std::string_view port, DWORD timeout, bool serversHTTPS,
			const json::utility::jsonObject& heuristic, utility::LoadSource loadSource,
			const std::unordered_map<std::string, std::vector<int64_t>>& allServers,
			std::shared_ptr<ResourceExecutor> resources,
			uint32_t processingThreads,
			uint32_t loadBalancingTargetRPS
		);

		~LoadBalancerServer() = default;
	};
}
