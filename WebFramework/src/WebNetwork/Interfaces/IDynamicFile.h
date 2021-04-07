#pragma once

#include <unordered_map>

#include "WebNetwork/HTTPResponse.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	namespace interfaces
	{
		/// <summary>
		/// Providing sendDynamicFile method
		/// </summary>
		class IDynamicFile
		{
		public:
			/// <summary>
			/// Sending file from filePath path
			/// </summary>
			/// <param name="filePath">must start with leading /</param>
			/// <param name="response">with file</param>
			virtual void sendDynamicFile(const std::string& filePath, HTTPResponse& response, const smartPointer<std::unordered_map<std::string_view, std::string>>& variables) = 0;

			virtual ~IDynamicFile() = default;
		};
	}
}
