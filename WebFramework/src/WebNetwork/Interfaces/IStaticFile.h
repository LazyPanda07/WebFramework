#pragma once

#include "WebNetwork/HTTPResponse.h"

namespace framework
{
	namespace interfaces
	{
		class IStaticFile
		{
		public:
			virtual void sendStaticFile(const std::string& filePath, HTTPResponse& response, bool isBinary) = 0;

			virtual ~IStaticFile() = default;
		};
	}
}
