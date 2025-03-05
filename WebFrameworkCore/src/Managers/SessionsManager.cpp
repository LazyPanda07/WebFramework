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

	void SessionsManager::SessionTime::updateSessionTime(const string& ip)
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

		ipTime[ip] = start;

		timeIp.emplace(move(start), ip);
	}

	SessionsManager::SessionsManager() :
		time(this)
	{

	}

	void SessionsManager::setAttribute(const string& ip, const string& name, const string& value)
	{
		unique_lock<mutex> guard(lock);

		userSession[ip][name] = value;

		time.updateSessionTime(ip);
	}

	string SessionsManager::getAttribute(const string& ip, const string& name)
	{
		unique_lock guard(lock);

		time.updateSessionTime(ip);

		return userSession.at(ip).at(name);
	}

	void SessionsManager::deleteSession(const string& ip)
	{
		unique_lock guard(lock);

		userSession.erase(ip);
	}

	void SessionsManager::deleteAttribute(const string& ip, const string& name)
	{
		unique_lock guard(lock);

		userSession[ip].erase(name);
	}
}
