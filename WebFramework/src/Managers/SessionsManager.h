#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <mutex>
#include <chrono>

namespace framework
{
	class SessionsManager
	{
	private:
		class SessionTime
		{
		public:
			using sessionTimePoint = std::chrono::high_resolution_clock::time_point;

		private:
			std::multimap<sessionTimePoint, std::string, std::greater_equal<sessionTimePoint>> timeIp;
			std::unordered_map<std::string, sessionTimePoint> ipTime;
			std::mutex checkLock;
			SessionsManager* userSessionSynchronization;

		private:
			void asyncCheck();

			void runAsyncCheck();

			void nextPeriod();

		public:
			SessionTime(SessionsManager* userSession);

			void updateSessionTime(const std::string& ip);

			~SessionTime() = default;
		};

	private:
		SessionTime time;
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> userSession;	//ip - (key - value)
		std::mutex lock;

	public:
		SessionsManager();

		void setAttribute(const std::string& ip, const std::string& name, const std::string& value);

		std::string getAttribute(const std::string& ip, const std::string& name);

		void deleteSession(const std::string& ip);

		void deleteAttribute(const std::string& ip, const std::string& name);

		~SessionsManager() = default;
	};
}
