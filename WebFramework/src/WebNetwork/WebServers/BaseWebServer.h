#pragma once

#include "BaseTCPServer.h"

#include <openssl/ssl.h>

#include "Framework/WebFrameworkPlatform.h"
#include "Utility/JSONSettingsParser.h"

namespace framework
{
	class BaseWebServer : public virtual web::BaseTCPServer
	{
	protected:
		SSL_CTX* context;
		bool useHTTPS;

	public:
		BaseWebServer();

		virtual ~BaseWebServer();
	};
}
