#include "BaseWebServer.h"

#include "Utility/Singletons/HTTPSSingleton.h"
#include "Exceptions/SSLException.h"

using namespace std;

namespace framework
{
	BaseWebServer::BaseWebServer() :
		context(nullptr)
	{
		utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();

		if (useHTTPS = httpsSettings.getUseHTTPS(); useHTTPS)
		{
			context = SSL_CTX_new(TLS_server_method());

			if (!context)
			{
				throw web::exceptions::SSLException(__LINE__, __FILE__);
			}

			if (int errorCode = SSL_CTX_use_certificate_file(context, httpsSettings.getPathToCertificate().string().data(), SSL_FILETYPE_PEM); errorCode != 1)
			{
				throw web::exceptions::SSLException(__LINE__, __FILE__, errorCode);
			}

			if (int errorCode = SSL_CTX_use_PrivateKey_file(context, httpsSettings.getPathToKey().string().data(), SSL_FILETYPE_PEM); errorCode != 1)
			{
				throw web::exceptions::SSLException(__LINE__, __FILE__, errorCode);
			}
		}
	}

	BaseWebServer::~BaseWebServer()
	{
		if (useHTTPS)
		{
			SSL_CTX_free(context);
		}
	}
}
