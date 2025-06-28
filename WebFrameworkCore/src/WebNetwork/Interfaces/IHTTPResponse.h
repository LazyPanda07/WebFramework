#pragma once

#include "WebFrameworkCoreConstants.h"

namespace framework::interfaces
{
	class WEB_FRAMEWORK_CORE_API IHTTPResponse
	{
	public:
		IHTTPResponse() = default;

		virtual void setHTTPVersion(const char* version) = 0;

		virtual void setResponseCode(int64_t code) = 0;

		virtual void addHeader(const char* name, const char* value) = 0;

		virtual void setBody(const char* body) = 0;

		virtual IHTTPResponse* appendBody(const char* body) = 0;

		virtual void addCookie(const char* name, const char* value) = 0;

		virtual void setDefault() = 0;

		virtual void setIsValid(bool isValid) = 0;

		virtual bool getIsValid() const = 0;

		virtual ~IHTTPResponse() = default;
	};
}
