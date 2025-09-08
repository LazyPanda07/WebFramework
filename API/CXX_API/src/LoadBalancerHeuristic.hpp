#pragma once

#include <string>
#include <cstdint>

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
#define WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API extern "C" __attribute__((visibility("default"))) __attribute__((used))
#else
#define WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API extern "C" __declspec(dllexport)
#endif

/**
* Macro for each LoadBalancerHeuristic subclass
* Used for loading function that creates LoadBalancerHeuristic subclass
*/
#define DEFINE_HEURISTIC(subclassName) WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API void* create##subclassName##CXXHeuristic(const char* ip, const char* port, bool useHTTPS)	\
{	\
	return new subclassName(ip, port, useHTTPS);	\
}

#pragma region ExportFunctions
WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API inline uint64_t webFrameworkCXXHeuristicOperator(void* implementation)
{
	return (*static_cast<framework::LoadBalancerHeuristic*>(implementation))();
}

WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API inline void webFrameworkCXXHeuristicOnStart(void* implementation)
{
	static_cast<framework::LoadBalancerHeuristic*>(implementation)->onStart();
}

WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API inline void webFrameworkCXXHeuristicOnEnd(void* implementation)
{
	static_cast<framework::LoadBalancerHeuristic*>(implementation)->onEnd();
}

WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API inline const char* webFrameworkCXXHeuristicGetIp(void* implementation)
{
	return static_cast<framework::LoadBalancerHeuristic*>(implementation)->getIp().data();
}

WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API inline const char* webFrameworkCXXHeuristicGetPort(void* implementation)
{
	return static_cast<framework::LoadBalancerHeuristic*>(implementation)->getPort().data();
}

WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API inline bool webFrameworkCXXHeuristicGetUseHTTPS(void* implementation)
{
	return static_cast<framework::LoadBalancerHeuristic*>(implementation)->getUseHTTPS();
}

WEB_FRAMEWORK_HEURISTIC_FUNCTIONS_API inline void webFrameworkCXXDeleteHeuristic(void* implementation)
{
	delete static_cast<framework::LoadBalancerHeuristic*>(implementation);
}
#pragma endregion
