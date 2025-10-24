#pragma once

#include "IFile.h"

#include <span>
#include <any>

#include "IHTTPRequest.h"

namespace framework::interfaces
{
	class IDynamicFile : virtual public IFile
	{
	public:
		/**
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header
		*/
		virtual void sendDynamicFile(std::string_view filePath, IHTTPResponse& response, std::span<const CVariable> variables, bool isBinary, std::string_view fileName) = 0;

		virtual void processWFDPFile(std::string& data, std::span<const CVariable> variables) = 0;

		virtual void registerDynamicFunction(std::string_view functionName, std::string_view apiType, const std::any& function) = 0;

		virtual void unregisterDynamicFunction(std::string_view functionName) = 0;

		virtual bool isDynamicFunctionRegistered(std::string_view functionName) = 0;

		virtual ~IDynamicFile() = default;
	};
}
