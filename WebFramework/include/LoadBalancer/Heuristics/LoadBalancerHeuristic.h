#pragma once

#include <string>
#include <cstdint>

#include "Framework/WebFrameworkPlatform.h"

namespace framework::load_balancer
{
	/**
	 * @brief Determine which server use
	 */
	class WEB_FRAMEWORK_EXPORT_API LoadBalancerHeuristic
	{
	public:
		LoadBalancerHeuristic() = default;

		/**
		 * @brief Calculate load score(choose server with lowest score)
		 * @return Load score
		 */
		virtual uint64_t operator ()() const = 0;

		/**
		 * @brief On start event
		 */
		virtual void onStart();

		/**
		 * @brief On end event
		 */
		virtual void onEnd();

		/**
		 * @brief Get server ip
		 * @return
		 */
		virtual const std::string& getIp() const = 0;

		/**
		 * @brief Get server port
		 * @return
		 */
		virtual const std::string& getPort() const = 0;

		/**
		 * @brief Is server using HTTPS
		 * @return
		 */
		virtual bool getUseHTTPS() const = 0;

		virtual ~LoadBalancerHeuristic() = default;
	};

	using CreateHeuristicFunction = void* (*)(const char* ip, const char* port, bool useHTTPS);
}

#ifdef __LINUX__
/**
* Macro for each LoadBalancerHeuristic subclass
* Used for loading function that creates LoadBalancerHeuristic subclass
*/
#define DEFINE_HEURISTIC(subclassName) extern "C" __attribute__((visibility("default"))) void* create##subclassName##Heuristic(std::string_view ip, std::string_view port, bool useHTTPS)	\
{	\
	return new subclassName(ip, port, useHTTPS);	\
}
#else
/**
* Macro for each LoadBalancerHeuristic subclass
* Used for loading function that creates LoadBalancerHeuristic subclass
*/
#define DEFINE_HEURISTIC(subclassName) extern "C" __declspec(dllexport) void* create##subclassName##Heuristic(std::string_view ip, std::string_view port, bool useHTTPS)	\
{	\
	return new subclassName(ip, port, useHTTPS);	\
}
#endif
