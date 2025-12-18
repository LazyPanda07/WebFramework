#include "HTTPResponseExecutors.h"

namespace framework
{
	interfaces::IHTTPResponse* HTTPResponseExecutors::getImplementation() const
	{
		return implementation;
	}

	HTTPResponseExecutors::HTTPResponseExecutors(interfaces::IHTTPResponse* implementation, const std::function<void(interfaces::IHTTPResponse*)>& deleter) :
		implementation(implementation),
		deleter(deleter)
	{

	}

	HTTPResponseExecutors::HTTPResponseExecutors(HTTPResponseExecutors&& other) noexcept
	{
		(*this) = std::move(other);
	}

	HTTPResponseExecutors& HTTPResponseExecutors::operator =(HTTPResponseExecutors&& other) noexcept
	{
		implementation = other.implementation;
		deleter = other.deleter;

		other.implementation = nullptr;
		other.deleter = nullptr;

		return *this;
	}

	void HTTPResponseExecutors::setHTTPVersion(std::string_view version)
	{
		implementation->setHTTPVersion(version.data());
	}

	void HTTPResponseExecutors::setResponseCode(web::ResponseCodes code)
	{
		implementation->setResponseCode(static_cast<int>(code));
	}

	void HTTPResponseExecutors::addHeader(std::string_view name, std::string_view value)
	{
		implementation->addHeader(name.data(), value.data());
	}

	void HTTPResponseExecutors::setBody(std::string_view body)
	{
		implementation->setBody(body.data());
	}

	void HTTPResponseExecutors::setBody(const json::JsonBuilder& json)
	{
		implementation->addHeader("Content-Type", "application/json");

		implementation->setBody(json.build().data());
	}

	HTTPResponseExecutors& HTTPResponseExecutors::appendBody(std::string_view body)
	{
		implementation->appendBody(body.data());

		return *this;
	}

	void HTTPResponseExecutors::addCookie(std::string_view name, std::string_view value)
	{
		implementation->addCookie(name.data(), value.data());
	}

	void HTTPResponseExecutors::setDefault()
	{
		implementation->setDefault();
	}

	void HTTPResponseExecutors::setIsValid(bool isValid)
	{
		implementation->setIsValid(isValid);
	}

	HTTPResponseExecutors::operator bool() const
	{
		return implementation->getIsValid();
	}

	HTTPResponseExecutors::~HTTPResponseExecutors()
	{
		if (deleter && implementation)
		{
			deleter(implementation);

			implementation = nullptr;
		}
	}
}
