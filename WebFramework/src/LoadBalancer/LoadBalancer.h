#pragma once

#include <filesystem>

#include "LoadBalancerWebNetwork/LoadBalancerServer.h"

namespace framework
{
	namespace load_balancer
	{
		class LoadBalancer
		{
		private:
			std::unique_ptr<LoadBalancerServer> loadBalancerServer;

		public:
			LoadBalancer(const std::filesystem::path& configurationINIFile);

			void startLoadBalancer();

			void stopLoadBalancerServer();

			bool getLoadBalancerState() const;

			~LoadBalancer() = default;
		};
	}
}
