#pragma once

#include "headers.h"

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

			/// @brief Add new function in .wfdp interpreter
			/// @param functionName Name of new function
			/// @param function Function implementation
			virtual void registerDynamicFunction(const std::string& functionName, std::function<std::string(const std::vector<std::string>&)>&& function) = 0;

			/// @brief Remove function from .wfdp interpreter
			/// @param functionName Name of function
			virtual void unregisterDynamicFunction(const std::string& functionName) = 0;

			/// @brief Check if function is registered
			/// @param functionName Name of function
			/// @return true if function is registered, false otherwise
			virtual bool isDynamicFunctionRegistered(const std::string& functionName) = 0;

			virtual ~IDynamicFile() = default;
		};
	}
}
