#pragma once

#include "BaseLoadBalancerHeuristic.h"

#ifdef __LINUX__
using HMODULE = void*;
#endif

namespace framework::load_balancer
{
	class CXXHeuristic : public BaseLoadBalancerHeuristic
	{
	private:
		using OperatorSignature = uint64_t(*)(void*);
		using OnStartSignature = void(*)(void*);
		using OnEndSignature = void(*)(void*);
		using GetIpSignature = const char* (*)(void*);
		using GetPortSignature = const char* (*)(void*);
		using GetUseHTTPSSignature = bool(*)(void*);
		using DeleteHeuristicSignature = void(*)(void*);

	private:
		std::string ip;
		std::string port;
		void* implementation;
		OperatorSignature operatorFunction;
		OnStartSignature onStartFunction;
		OnEndSignature onEndFunction;
		GetIpSignature getIpFunction;
		GetPortSignature getPortFunction;
		GetUseHTTPSSignature getUseHTTPSFunction;
		DeleteHeuristicSignature deleteHeuristicFunction;

	public:
		CXXHeuristic(std::string_view ip, std::string_view port, bool useHTTPS, std::string_view heuristicName, HMODULE handle);

		uint64_t operator ()() const override;

		void onStart() override;

		void onEnd() override;

		const std::string& getIp() const override;

		const std::string& getPort() const override;

		bool getUseHTTPS() const override;

		~CXXHeuristic();
	};
}
