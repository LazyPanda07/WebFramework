#pragma once

#include "IFile.h"
#include "IHTTPResponse.h"

namespace framework::interfaces
{
	class IStaticFile : virtual public IFile
	{
	public:
		/**
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header
		*/
		virtual void sendStaticFile(std::string_view filePath, IHTTPResponse& response, bool isBinary, std::string_view fileName) = 0;

		virtual ~IStaticFile() = default;
	};
}
