#include "HTTPSSingleton.h"

using namespace std;

namespace framework
{
	namespace utility
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

		void HTTPSSingleton::setPathToCertificate(const filesystem::path& pathToCertificate)
		{
			this->pathToCertificate = pathToCertificate;
		}

		void HTTPSSingleton::setPathToKey(const filesystem::path& pathToKey)
		{
			this->pathToKey = pathToKey;
		}

		bool HTTPSSingleton::getUseHTTPS() const
		{
			return useHTTPS;
		}

		const filesystem::path& HTTPSSingleton::getPathToCertificate() const
		{
			return pathToCertificate;
		}

		const filesystem::path& HTTPSSingleton::getPathToKey() const
		{
			return pathToKey;
		}
	}
}
