#pragma once

#include "BaseLoadBalancerHeuristic.h"

namespace framework::load_balancer
{
	class CXXHeuristic : public BaseLoadBalancerHeuristic
	{
	private:
		void* implementation;

	public:
		CXXHeuristic(void* implementation, std::string_view ip, std::string_view port, bool useHTTPS);

		uint64_t operator ()() const override;

		void onStart() override;

		void onEnd() override;

		const std::string& getIp() const override;

		const std::string& getPort() const override;

		bool getUseHTTPS() const override;

		~CXXHeuristic();
	};
}
