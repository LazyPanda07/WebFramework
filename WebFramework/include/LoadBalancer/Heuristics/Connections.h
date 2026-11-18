#pragma once

#include "LoadBalancerHeuristic.h"

#include <atomic>

namespace framework::load_balancer
{
	class WEB_FRAMEWORK_API Connections : public LoadBalancerHeuristic
	{
	private:
		std::string ip;
		std::string port;
		std::atomic_uint64_t connections;

	public:
		Connections(std::string_view ip, std::string_view port);

		uint64_t operator ()() const override;

		void onStart() override;

		void onEnd() override;

		const std::string& getIp() const override;

		const std::string& getPort() const override;

		~Connections() = default;
	};
}
