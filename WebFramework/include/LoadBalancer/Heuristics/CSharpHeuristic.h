#pragma once

#include "LoadBalancerHeuristic.h"

#ifdef __WITH_DOTNET_EXECUTORS__

#include "Utility/Sources.h"

namespace framework::load_balancer
{
	class CSharpHeuristic : public LoadBalancerHeuristic
	{
	private:
		std::string ip;
		std::string port;
		bool useHTTPS;
		void* implementation;

	public:
		CSharpHeuristic(std::string_view ip, std::string_view port, bool useHTTPS, std::string_view heuristicName, const utility::LoadSource& source);

		uint64_t operator ()() const override;

		void onStart() override;

		void onEnd() override;

		const std::string& getIp() const override;

		const std::string& getPort() const override;

		bool getUseHTTPS() const override;

		~CSharpHeuristic();
	};
}

#endif
