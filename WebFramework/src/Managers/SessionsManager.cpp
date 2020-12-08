#include "SessionsManager.h"

#include "WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	void SessionsManager::SessionTime::asyncCheck()
	{
		lock_guard<mutex> guard(checkLock);
		vector<pair<sessionTimePoint, string>> deleteVariants;
		sessionTimePoint current = chrono::high_resolution_clock::now();

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

		nextPeriod();
	}

	void SessionsManager::SessionTime::runAsyncCheck()
	{
		while (true)
		{
			asyncCheck();

			this_thread::sleep_for(sessionCheckPeriod);
		}
	}

	void SessionsManager::SessionTime::nextPeriod()
	{
		multimap<sessionTimePoint, string, greater_equal<sessionTimePoint>> tem;
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
		userSessionSynchronization(userSession)
	{
		thread(&SessionTime::runAsyncCheck, this).detach();
	}

	void SessionsManager::SessionTime::updateSessionTime(const string& ip)
	{
		lock_guard<mutex> guard(checkLock);

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
		lock_guard<mutex> guard(lock);

		userSession[ip][name] = value;

		time.updateSessionTime(ip);
	}

	string SessionsManager::getAttribute(const string& ip, const string& name)
	{
		lock_guard guard(lock);

		time.updateSessionTime(ip);

		return userSession.at(ip).at(name);
	}

	void SessionsManager::deleteSession(const string& ip)
	{
		lock_guard guard(lock);

		userSession.erase(ip);
	}

	void SessionsManager::deleteAttribute(const string& ip, const string& name)
	{
		lock_guard guard(lock);

		userSession[ip].erase(name);
	}
}
