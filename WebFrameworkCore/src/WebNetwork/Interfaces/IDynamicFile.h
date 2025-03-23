#pragma once

#include "IFile.h"
#include "WebNetwork/HTTPResponse.h"

namespace framework
{
	namespace interfaces
	{
		class WEB_FRAMEWORK_CORE_API IDynamicFile : virtual public IFile
		{
		public:
			/**
			* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header
			*/
			virtual void sendDynamicFile(std::string_view filePath, HTTPResponse& response, const std::unordered_map<std::string, std::string>& variables, bool isBinary, std::string_view fileName) = 0;

			virtual void registerDynamicFunction(std::string_view functionName, std::function<std::string(const std::vector<std::string>&)>&& function) = 0;

			virtual void unregisterDynamicFunction(std::string_view functionName) = 0;

			virtual bool isDynamicFunctionRegistered(std::string_view functionName) = 0;

			virtual ~IDynamicFile() = default;
		};
	}
}
