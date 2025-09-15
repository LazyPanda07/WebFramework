#include "SessionsManager.h"

#include <thread>

using namespace std;

namespace framework
{
	void SessionsManager::SessionTime::asyncCheck()
	{
		unique_lock<mutex> guard(checkLock);
		vector<pair<SessionTimePoint, string>> deleteVariants;
		const SessionTimePoint current = chrono::high_resolution_clock::now();

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

			this_thread::sleep_for(sessionCheckPeriod);
		}
	}

	void SessionsManager::SessionTime::nextPeriod()
	{
		multimap<SessionTimePoint, string, greater<SessionTimePoint>> tem;
		const SessionTimePoint period = chrono::high_resolution_clock::now();

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
		thread(&SessionTime::runAsyncCheck, this).detach();
	}

	void SessionsManager::SessionTime::updateSessionTime(string_view ip)
	{
		unique_lock<mutex> guard(checkLock);

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

		SessionTimePoint start = chrono::high_resolution_clock::now();

		ipTime.emplace(ip, start);
		timeIp.emplace(move(start), ip);
	}

	SessionsManager::SessionsManager() :
		time(this)
	{

	}

	void SessionsManager::setAttribute(const string& ip, string_view name, string_view value)
	{
		unique_lock<mutex> guard(lock);

		userSession[ip].emplace(name, value);

		time.updateSessionTime(ip);
	}

	string SessionsManager::getAttribute(string_view ip, string_view name)
	{
		unique_lock guard(lock);

		time.updateSessionTime(ip);

		if (auto ipIt = userSession.find(ip); ipIt != userSession.end())
		{
			if (auto it = ipIt->second.find(name); it != ipIt->second.end())
			{
				return it->second;
			}
			
			throw runtime_error(format("Wrong attribute name: {}", name));
		}

		throw runtime_error(format("Can't find ip: {}", ip));

		return {};
	}

	void SessionsManager::deleteSession(string_view ip)
	{
		unique_lock guard(lock);

		userSession.erase(userSession.find(ip));
	}

	void SessionsManager::deleteAttribute(string_view ip, string_view name)
	{
		unique_lock guard(lock);

		if (auto it = userSession.find(ip); it != userSession.end())
		{
			if (auto eraseIt = it->second.find(name); eraseIt != it->second.end())
			{
				it->second.erase(eraseIt);
			}
		}
	}
}
