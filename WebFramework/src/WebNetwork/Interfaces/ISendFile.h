#pragma once

#include "../HTTPResponse.h"

namespace framework
{
	namespace interfaces
	{
		struct ISendFile
		{
			virtual void sendFile(const std::string& filePath, HTTPResponse& response) = 0;

			virtual ~ISendFile() = default;
		};
	}
}