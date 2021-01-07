#pragma once

#include "../HTTPResponse.h"

namespace framework
{
	namespace interfaces
	{
		/// <summary>
		/// Providing sendFile method
		/// </summary>
		class ISendStaticFile
		{
		public:
			/// <summary>
			/// Sending file from filePath path
			/// </summary>
			/// <param name="filePath">must start with leading /</param>
			/// <param name="response">with file</param>
			virtual void sendFile(const std::string& filePath, HTTPResponse& response) = 0;

			virtual ~ISendStaticFile() = default;
		};
	}
}
