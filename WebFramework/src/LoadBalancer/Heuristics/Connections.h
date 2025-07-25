#pragma once

#include "BaseLoadBalancerHeuristic.h"

#include <atomic>

namespace framework
{
	namespace load_balancer
	{
		class Connections : public BaseLoadBalancerHeuristic
		{
		private:
			std::string ip;
			std::string port;
			bool useHTTPS;
			std::atomic_uint64_t connections;

		public:
			Connections(std::string_view ip, std::string_view port, bool useHTTPS);

			uint64_t operator ()() const override;

			void onStart() override;

			void onEnd() override;

			const std::string& getIp() const override;
			
			const std::string& getPort() const override;

			bool getUseHTTPS() const override;

			~Connections() = default;
		};
	}
}
