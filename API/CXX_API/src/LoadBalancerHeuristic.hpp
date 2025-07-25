#pragma once

#include <string>
#include <cstdint>

#include "APIExecutors/BaseExecutor.hpp"

namespace framework
{
	/**
	 * @brief Determine which server use
	 */
	class LoadBalancerHeuristic
	{
	private:
		std::string ip;
		std::string port;
		bool useHTTPS;

	public:
		LoadBalancerHeuristic(std::string_view ip, std::string_view port, bool useHTTPS);

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

		virtual ~LoadBalancerHeuristic() = default;
	};
}

namespace framework
{
	inline LoadBalancerHeuristic::LoadBalancerHeuristic(std::string_view ip, std::string_view port, bool useHTTPS) :
		ip(ip),
		port(port),
		useHTTPS(useHTTPS)
	{

	}

	inline void LoadBalancerHeuristic::onStart()
	{

	}

	inline void LoadBalancerHeuristic::onEnd()
	{

	}

	inline const std::string& LoadBalancerHeuristic::getIp() const
	{
		return ip;
	}

	inline const std::string& LoadBalancerHeuristic::getPort() const
	{
		return port;
	}

	inline bool LoadBalancerHeuristic::getUseHTTPS() const
	{
		return useHTTPS;
	}
}

#ifdef __LINUX__
/**
* Macro for each LoadBalancerHeuristic subclass
* Used for loading function that creates LoadBalancerHeuristic subclass
*/
#define DECLARE_HEURISTIC(subclassName) extern "C" __attribute__((visibility("default"))) __attribute__((used)) void* create##subclassName##Heuristic(std::string_view ip, std::string_view port, bool useHTTPS)	\
{	\
	return new subclassName(ip, port, useHTTPS);	\
}
#else
/**
* Macro for each LoadBalancerHeuristic subclass
* Used for loading function that creates LoadBalancerHeuristic subclass
*/
#define DECLARE_HEURISTIC(subclassName) extern "C" __declspec(dllexport) void* create##subclassName##Heuristic(std::string_view ip, std::string_view port, bool useHTTPS)	\
{	\
	return new subclassName(ip, port, useHTTPS);	\
}
#endif

#pragma region ExportFunctions
WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline uint64_t webFrameworkHeuristicOperator(void* implementation)
{
	return (*static_cast<framework::LoadBalancerHeuristic*>(implementation))();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkHeuristicOnStart(void* implementation)
{
	static_cast<framework::LoadBalancerHeuristic*>(implementation)->onStart();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkHeuristicOnEnd(void* implementation)
{
	static_cast<framework::LoadBalancerHeuristic*>(implementation)->onEnd();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline const char* webFrameworkHeuristicGetIp(void* implementation)
{
	return static_cast<framework::LoadBalancerHeuristic*>(implementation)->getIp().data();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline const char* webFrameworkHeuristicGetPort(void* implementation)
{
	return static_cast<framework::LoadBalancerHeuristic*>(implementation)->getPort().data();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline bool webFrameworkHeuristicGetUseHTTPS(void* implementation)
{
	return static_cast<framework::LoadBalancerHeuristic*>(implementation)->getUseHTTPS();
}

WEB_FRAMEWORK_EXECUTOR_FUNCTIONS_API inline void webFrameworkDeleteHeuristic(void* implementation)
{
	delete static_cast<framework::LoadBalancerHeuristic*>(implementation);
}
#pragma endregion
