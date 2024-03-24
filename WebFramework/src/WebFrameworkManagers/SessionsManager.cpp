#include "SessionsManager.h"

using namespace std;

namespace framework
{
	void SessionsManager::SessionTime::asyncCheck()
	{
		unique_lock<mutex> guard(checkLock);
		vector<pair<sessionTimePoint, string>> deleteVariants;
		const sessionTimePoint current = chrono::high_resolution_clock::now();

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

		for (const auto& i : deleteVariants)
		{
			timeIp.erase(i.first);
			ipTime.erase(i.second);

			userSessionSynchronization->deleteSession(i.second);
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
		multimap<sessionTimePoint, string, greater<sessionTimePoint>> tem;
		const sessionTimePoint period = chrono::high_resolution_clock::now();

		for (const auto& i : timeIp)
		{
			sessionTimePoint next = i.first;

			next += period - next;

			ipTime[i.second] = next;

			tem.insert(make_pair(move(next), i.second));
		}

		timeIp = move(tem);
	}

	SessionsManager::SessionTime::SessionTime(SessionsManager* userSession) :
		userSessionSynchronization(userSession),
		handle(async(launch::async, &SessionTime::runAsyncCheck, this))
	{
		
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

		sessionTimePoint start = chrono::high_resolution_clock::now();

		ipTime[ip] = start;

		timeIp.insert(make_pair(move(start), ip));
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
