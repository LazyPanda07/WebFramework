#pragma once

#include "headers.h"

#include "WebNetwork/HTTPResponse.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	namespace interfaces
	{
		class IDynamicFile
		{
		public:
			virtual void sendDynamicFile(const std::string& filePath, HTTPResponse& response, const std::unordered_map<std::string_view, std::string>& variables, bool isBinary) = 0;

			virtual void registerDynamicFunction(const std::string& functionName, std::function<std::string(const std::vector<std::string>&)>&& function) = 0;

			virtual void unregisterDynamicFunction(const std::string& functionName) = 0;

			virtual bool isDynamicFunctionRegistered(const std::string& functionName) = 0;

			virtual ~IDynamicFile() = default;
		};
	}
}
