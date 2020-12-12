#pragma once

#include "../HTTPResponse.h"

namespace framework
{
	namespace interfaces
	{
		class ISendFile
		{
		public:
			//filePath must be with leading /
			virtual void sendFile(const std::string& filePath, HTTPResponse& response) = 0;

			virtual ~ISendFile() = default;
		};
	}
}