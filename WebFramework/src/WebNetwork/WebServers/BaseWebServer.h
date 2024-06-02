#pragma once

#include "WebFrameworkCore.h"

#include "Utility/JSONSettingsParser.h"
#include "WebNetwork/Interfaces/IExecutorFunctionality.h"

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
