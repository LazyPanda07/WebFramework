#include "HTTPSSingleton.h"

namespace framework::utility
{
	HTTPSSingleton::HTTPSSingleton() :
		useHTTPS(false)
	{

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
	}

	void HTTPSSingleton::setPathToKey(const std::filesystem::path& pathToKey)
	{
		this->pathToKey = pathToKey;
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
