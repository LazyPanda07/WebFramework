#pragma once

#include "IFile.h"
#include "WebNetwork/HTTPResponse.h"

namespace framework
{
	namespace interfaces
	{
		class WEB_FRAMEWORK_API IStaticFile : virtual public IFile
		{
		public:
			/**
			* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header
			*/
			virtual void sendStaticFile(const std::string& filePath, HTTPResponse& response, bool isBinary, const std::string& fileName) = 0;

			virtual ~IStaticFile() = default;
		};
	}
}
