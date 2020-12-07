#include "SessionsManager.h"

#include "WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	void SessionsManager::SessionTime::asyncCheck()
	{
		lock_guard<mutex> guard(checkLock);
		vector<pair<unsigned int, string>> deleteVariants;

		for (const auto& i : timeIp)
		{
			if (i.first >= sessionLifeTime)
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
		multimap<unsigned int, string, greater<unsigned int>> tem;
		static unsigned int period = sessionCheckPeriod.count();

		for (const auto& i : timeIp)
		{
			tem.insert(make_pair(i.first + period, i.second));

			ipTime[i.second] = i.first + period;
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

		timeIp.insert(make_pair(0U, ip));

		ipTime[ip] = 0U;

	}

	SessionsManager::SessionsManager() :
		time(this)
	{

	}

	void SessionsManager::setAttribute(const string& ip, const string& name, const string& value)
	{
		lock_guard<mutex> guard(lock);

		userSession[ip][name] = value;
	}

	string SessionsManager::getAttribute(const string& ip, const string& name)
	{
		lock_guard guard(lock);

		return userSession.at(ip).at(name);
	}

	void SessionsManager::deleteSession(const string& ip)
	{
		lock_guard guard(lock);

		userSession.erase(ip);
	}
}
