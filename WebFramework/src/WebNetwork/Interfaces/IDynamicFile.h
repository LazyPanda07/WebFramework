#pragma once

#include "core.h"

#include "IFile.h"
#include "WebNetwork/HTTPResponse.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	namespace interfaces
	{
		class IDynamicFile : virtual public IFile
		{
		public:
			/**
			* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header
			*/
			virtual void sendDynamicFile(const std::string& filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables, bool isBinary, const std::string& fileName) = 0;

			virtual void registerDynamicFunction(const std::string& functionName, std::function<std::string(const std::vector<std::string>&)>&& function) = 0;

			virtual void unregisterDynamicFunction(const std::string& functionName) = 0;

			virtual bool isDynamicFunctionRegistered(const std::string& functionName) = 0;

			virtual ~IDynamicFile() = default;
		};
	}
}
