#pragma once

#include <any>

#include "IHttpRequest.h"

namespace framework::interfaces
{
	class IDynamicFile
	{
	public:
		/**
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header
		*/
		virtual void sendDynamicFile(std::string_view filePath, IHttpResponse& response, const void* arguments, std::string_view fileName) = 0;

		virtual void processDynamicFile(std::string& data, const void* arguments) = 0;

		virtual void registerDynamicFunction(std::string_view functionName, std::string_view apiType, const std::any& function) = 0;

		virtual void unregisterDynamicFunction(std::string_view functionName) = 0;

		virtual bool isDynamicFunctionRegistered(std::string_view functionName) = 0;

		virtual ~IDynamicFile() = default;
	};
}
