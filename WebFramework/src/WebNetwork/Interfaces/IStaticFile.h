#pragma once

#include "WebNetwork/HTTPResponse.h"

namespace framework
{
	namespace interfaces
	{
		/// <summary>
		/// Providing sendStaticFile method
		/// </summary>
		class IStaticFile
		{
		public:
			/// <summary>
			/// Sending file from filePath path
			/// </summary>
			/// <param name="filePath">must start with leading /</param>
			/// <param name="response">with file</param>
			virtual void sendStaticFile(const std::string& filePath, HTTPResponse& response) = 0;

			virtual ~IStaticFile() = default;
		};
	}
}
