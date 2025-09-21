#include "SessionsManager.h"

#include <thread>

namespace framework
{
	void SessionsManager::SessionTime::asyncCheck()
	{
		std::unique_lock<std::mutex> guard(checkLock);
		std::vector<std::pair<SessionTimePoint, std::string>> deleteVariants;
		const SessionTimePoint current = std::chrono::high_resolution_clock::now();

		for (const auto& i : timeIp)
		{
			if ((current - i.first).count() >= sessionLifeTime)
			{
				deleteVariants.push_back(i);
			}
			else
			{
				break;
			}
		}

		for (const auto& [time, ip] : deleteVariants)
		{
			timeIp.erase(time);
			ipTime.erase(ip);

			userSessionSynchronization->deleteSession(ip);
		}

		this->nextPeriod();
	}

	void SessionsManager::SessionTime::runAsyncCheck()
	{
		while (true)
		{
			this->asyncCheck();

			std::this_thread::sleep_for(sessionCheckPeriod);
		}
	}

	void SessionsManager::SessionTime::nextPeriod()
	{
		std::multimap<SessionTimePoint, std::string, std::greater<SessionTimePoint>> tem;
		const SessionTimePoint period = std::chrono::high_resolution_clock::now();

		for (const auto& [time, ip] : timeIp)
		{
			SessionTimePoint next = time;

			next += period - next;

			ipTime[ip] = next;

			tem.emplace(move(next), ip);
		}

		timeIp = move(tem);
	}

	SessionsManager::SessionTime::SessionTime(SessionsManager* userSession) :
		userSessionSynchronization(userSession)
	{
		std::thread(&SessionTime::runAsyncCheck, this).detach();
	}

	void SessionsManager::SessionTime::updateSessionTime(std::string_view ip)
	{
		std::unique_lock<std::mutex> guard(checkLock);

		auto findTime = ipTime.find(ip);

		if (findTime != ipTime.end())
		{
			auto target = timeIp.find(findTime->second);

			while (target->second != ip)
			{
				target++;
			}

			timeIp.erase(target);
		}

		SessionTimePoint start = std::chrono::high_resolution_clock::now();

		ipTime.emplace(ip, start);
		timeIp.emplace(move(start), ip);
	}

	SessionsManager::SessionsManager() :
		time(this)
	{

	}

	void SessionsManager::setAttribute(const std::string& ip, std::string_view name, std::string_view value)
	{
		std::unique_lock<std::mutex> guard(lock);

		userSession[ip].emplace(name, value);

		time.updateSessionTime(ip);
	}

	std::string SessionsManager::getAttribute(std::string_view ip, std::string_view name)
	{
		std::unique_lock guard(lock);

		time.updateSessionTime(ip);

		if (auto ipIt = userSession.find(ip); ipIt != userSession.end())
		{
			if (auto it = ipIt->second.find(name); it != ipIt->second.end())
			{
				return it->second;
			}
			
			throw std::runtime_error(format("Wrong attribute name: {}", name));
		}

		throw std::runtime_error(format("Can't find ip: {}", ip));

		return {};
	}

	void SessionsManager::deleteSession(std::string_view ip)
	{
		std::unique_lock guard(lock);

		userSession.erase(userSession.find(ip));
	}

	void SessionsManager::deleteAttribute(std::string_view ip, std::string_view name)
	{
		std::unique_lock guard(lock);

		if (auto it = userSession.find(ip); it != userSession.end())
		{
			if (auto eraseIt = it->second.find(name); eraseIt != it->second.end())
			{
				it->second.erase(eraseIt);
			}
		}
	}
}
