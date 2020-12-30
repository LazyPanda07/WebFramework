#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <mutex>
#include <chrono>

namespace framework
{
	/// <summary>
	/// Providing and managing session mechanism
	/// </summary>
	class SessionsManager
	{
	private:
		/// <summary>
		/// Checking time for each session
		/// </summary>
		class SessionTime
		{
		public:
			using sessionTimePoint = std::chrono::high_resolution_clock::time_point;

		private:
			std::multimap<sessionTimePoint, std::string, std::greater<sessionTimePoint>> timeIp;
			std::unordered_map<std::string, sessionTimePoint> ipTime;
			std::mutex checkLock;
			SessionsManager* userSessionSynchronization;

		private:
			/// <summary>
			/// Asynchronous checking
			/// </summary>
			void asyncCheck();

			/// <summary>
			/// Start asynchronous checking
			/// </summary>
			void runAsyncCheck();

			/// <summary>
			/// Updating all timers
			/// </summary>
			void nextPeriod();

		public:
			/// <summary>
			/// Construct SessionTime
			/// </summary>
			/// <param name="userSession">pointer to outer class</param>
			SessionTime(SessionsManager* userSession);

			/// <summary>
			/// Adding new session
			/// </summary>
			/// <param name="ip">client's address</param>
			void updateSessionTime(const std::string& ip);

			~SessionTime() = default;
		};

	private:
		SessionTime time;
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> userSession;	//ip - (key - value)
		std::mutex lock;

	public:
		SessionsManager();

		/// <summary>
		/// Set session attribute for client
		/// </summary>
		/// <param name="ip">client's address</param>
		/// <param name="name">attribute name</param>
		/// <param name="value">attribute value</param>
		void setAttribute(const std::string& ip, const std::string& name, const std::string& value);

		/// <summary>
		/// Get attribute from session
		/// </summary>
		/// <param name="ip">client's address</param>
		/// <param name="name">attribute name</param>
		/// <returns>string value</returns>
		std::string getAttribute(const std::string& ip, const std::string& name);

		/// <summary>
		/// Delete client's session
		/// </summary>
		/// <param name="ip">client's address</param>
		void deleteSession(const std::string& ip);

		/// <summary>
		/// Delete attribute from client's session
		/// </summary>
		/// <param name="ip">client's address</param>
		/// <param name="name">attribute name</param>
		void deleteAttribute(const std::string& ip, const std::string& name);

		~SessionsManager() = default;
	};
}
