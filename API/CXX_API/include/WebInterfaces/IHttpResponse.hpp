#pragma once

#include <cstdint>

namespace framework::interfaces
{
	class IHttpResponse
	{
	public:
		IHttpResponse() = default;

		virtual void setHTTPVersion(const char* version) = 0;

		virtual void setResponseCode(int64_t code) = 0;

		virtual void addHeader(const char* name, const char* value) = 0;

		virtual void setBody(const char* body, size_t bodySize) = 0;

		virtual void setJsonBodyWithBuilder(void* implementation) = 0;

		virtual void setJsonBodyWithObject(void* implementation) = 0;

		virtual IHttpResponse* appendBody(const char* body) = 0;

		virtual void addCookie(const char* name, const char* value) = 0;

		virtual void setDefault() = 0;

		virtual void setIsValid(bool isValid) = 0;

		virtual bool getIsValid() const = 0;

		virtual ~IHttpResponse() = default;
	};
}
