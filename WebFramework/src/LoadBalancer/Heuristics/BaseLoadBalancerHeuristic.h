#pragma once

#include "Import/WebFrameworkCore.h"

#ifdef WEB_FRAMEWORK_DLL
#ifdef __LINUX__
#define WEB_FRAMEWORK_EXPORT_API __attribute__((visibility("default")))
#else
#define WEB_FRAMEWORK_EXPORT_API __declspec(dllexport)
#endif
#else
#define WEB_FRAMEWORK_EXPORT_API
#endif

namespace framework::load_balancer
{
	/**
	 * @brief Determine which server use
	 */
	class WEB_FRAMEWORK_EXPORT_API BaseLoadBalancerHeuristic
	{
	private:
		std::string ip;
		std::string port;
		bool useHTTPS;

	public:
		BaseLoadBalancerHeuristic(std::string_view ip, std::string_view port, bool useHTTPS);

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
		const std::string& getIp() const;

		/**
		 * @brief Get server port
		 * @return
		 */
		const std::string& getPort() const;

		/**
		 * @brief Is server using HTTPS
		 * @return
		 */
		bool getUseHTTPS() const;

		virtual ~BaseLoadBalancerHeuristic() = default;
	};

	using CreateHeuristicFunction = void* (*)(std::string_view ip, std::string_view port, bool useHTTPS);
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
