#include "Utility/Singletons/HTTPSSingleton.h"

#ifdef __LINUX__
#include <signal.h>
#endif

namespace framework::utility
{
	HTTPSSingleton::HTTPSSingleton() :
		useHTTPS(false)
	{
#ifdef __LINUX__
		signal(SIGPIPE, SIG_IGN);
#endif
	}

	HTTPSSingleton& HTTPSSingleton::get()
	{
		static HTTPSSingleton instance;

		return instance;
	}

	void HTTPSSingleton::setUseHTTPS(bool useHTTPS)
	{
		this->useHTTPS = useHTTPS;
	}

	void HTTPSSingleton::setPathToCertificate(const std::filesystem::path& pathToCertificate)
	{
		this->pathToCertificate = pathToCertificate;
		this->pathToCertificate = this->pathToCertificate.make_preferred();
	}

	void HTTPSSingleton::setPathToKey(const std::filesystem::path& pathToKey)
	{
		this->pathToKey = pathToKey;
		this->pathToKey = this->pathToKey.make_preferred();
	}

	bool HTTPSSingleton::getUseHTTPS() const
	{
		return useHTTPS;
	}

	const std::filesystem::path& HTTPSSingleton::getPathToCertificate() const
	{
		return pathToCertificate;
	}

	const std::filesystem::path& HTTPSSingleton::getPathToKey() const
	{
		return pathToKey;
	}
}
