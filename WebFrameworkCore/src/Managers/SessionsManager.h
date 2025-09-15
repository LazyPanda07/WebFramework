#pragma once

#include <chrono>
#include <map>
#include <mutex>
#include <unordered_map>

#include "WebFrameworkCoreConstants.h"
#include "Strings.h"

namespace framework
{
	class WEB_FRAMEWORK_CORE_API SessionsManager
	{
	private:
		class WEB_FRAMEWORK_CORE_API SessionTime
		{
		public:
			using SessionTimePoint = std::chrono::high_resolution_clock::time_point;

		private:
			std::multimap<SessionTimePoint, std::string, std::greater<SessionTimePoint>> timeIp;
			::utility::strings::string_based_unordered_map<SessionTimePoint> ipTime;
			std::mutex checkLock;
			SessionsManager* userSessionSynchronization;

		private:
			void asyncCheck();

			void runAsyncCheck();

			void nextPeriod();

		public:
			SessionTime(SessionsManager* userSession);

			void updateSessionTime(std::string_view ip);

			~SessionTime() = default;
		};

	private:
		SessionTime time;
		::utility::strings::string_based_unordered_map<::utility::strings::string_based_unordered_map<std::string>> userSession; // ip - (key - value)
		std::mutex lock;

	public:
		SessionsManager();

		void setAttribute(const std::string& ip, std::string_view name, std::string_view value);

		std::string getAttribute(std::string_view ip, std::string_view name);

		void deleteSession(std::string_view ip);

		void deleteAttribute(std::string_view ip, std::string_view name);

		~SessionsManager() = default;
	};
}
