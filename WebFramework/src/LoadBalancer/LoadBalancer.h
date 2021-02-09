#pragma once

#if defined(LOAD_BALANCER_DLL) || defined(ALL_DLL)
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif // LOAD_BALANCER_DLL


#include <filesystem>

#include "LoadBalancerWebNetwork/LoadBalancerServer.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	namespace load_balancer
	{
		/// <summary>
		/// <para>Main class of load balancer</para>
		/// <para>All initializations go through this class</para>
		/// </summary>
		class WEB_FRAMEWORK_API LoadBalancer
		{
		private:
			smartPointer<LoadBalancerServer> loadBalancerServer;

		public:
			/// <summary>
			/// Construct LoadBalancer
			/// </summary>
			/// <param name="configurationINIFile">path to .ini configuration file</param>
			/// <exception cref="std::out_of_range">Can't find some settings</exception>
			/// <exception cref="framework::exceptions::FileDoesNotExistException"></exception>
			LoadBalancer(const std::filesystem::path& configurationINIFile);

			/// <summary>
			/// Start LoadBalancerServer server
			/// </summary>
			void startLoadBalancer();

			/// <summary>
			/// Stop LoadBalancerServer incoming connections
			/// </summary>
			void stopLoadBalancerServer();

			/// <summary>
			/// Is server running
			/// </summary>
			/// <returns>is server running</returns>
			bool getLoadBalancerState() const;

			~LoadBalancer() = default;
		};
	}
}
