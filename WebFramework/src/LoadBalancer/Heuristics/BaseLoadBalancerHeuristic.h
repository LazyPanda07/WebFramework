#pragma once

#include "Import/WebFrameworkCore.h"

namespace framework
{
	namespace load_balancer
	{
		class WEB_FRAMEWORK_API BaseLoadBalancerHeuristic
		{
		private:
			std::string ip;
			std::string port;
			bool useHTTPS;

		public:
			BaseLoadBalancerHeuristic(std::string_view ip, std::string_view port, bool useHTTPS);

			virtual uint64_t operator ()() const = 0;

			virtual void onStart();

			virtual void onEnd();

			const std::string& getIp() const;

			const std::string& getPort() const;

			bool getUseHTTPS() const;

			virtual ~BaseLoadBalancerHeuristic() = default;
		};

		using createHeuristicFunction = void* (*)(std::string_view ip, std::string_view port, bool useHTTPS);
	}
}

#ifdef __LINUX__
/**
* Macro for each BaseLoadBalancerHeuristic subclass
* Used for loading function that creates BaseLoadBalancerHeuristic subclass
*/
#define DECLARE_HEURISTIC(subclassName) extern "C" __attribute__((visibility("default"))) void* create##subclassName##Heuristic(std::string_view ip, std::string_view port, bool useHTTPS)	\
{	\
	return new subclassName(ip, port, useHTTPS);	\
}
#else
/**
* Macro for each BaseLoadBalancerHeuristic subclass
* Used for loading function that creates BaseLoadBalancerHeuristic subclass
*/
#define DECLARE_HEURISTIC(subclassName) extern "C" __declspec(dllexport) void* create##subclassName##Heuristic(std::string_view ip, std::string_view port, bool useHTTPS)	\
{	\
	return new subclassName(ip, port, useHTTPS);	\
}
#endif
