#include "Web/Servers/BaseWebServer.h"

#include <Exceptions/SslException.h>
#include <Exceptions/FileDoesNotExistException.h>
#include <HttpsNetwork.h>
#include <openssl/err.h>

#include "Utility/Utils.h"
#include "Framework/WebFramework.h"

namespace framework
{
	void BaseWebServer::readSsl(const std::filesystem::path& filePath, const std::function<void(FILE*, const std::filesystem::path&)>& callback)
	{
		FILE* file = nullptr;

#ifdef __LINUX__
		file = fopen(filePath.string().data(), "rb");
#else
		fopen_s(&file, filePath.string().data(), "rb");
#endif

		if (!file)
		{
			throw file_manager::exceptions::FileDoesNotExistException(filePath);
		}

		try
		{
			callback(file, filePath);
		}
		catch (const std::exception&)
		{
			fclose(file);

			throw;
		}
	}

	bool BaseWebServer::autoCloseSocket() const
	{
		return false;
	}

	streams::IOSocketStream BaseWebServer::createServerSideStream(SOCKET clientSocket, SSL* ssl, std::chrono::milliseconds timeoutInMilliseconds)
	{
		if (ssl)
		{
			std::lock_guard<std::mutex> lock(sslMutex);

			return streams::IOSocketStream::createStream<web::HttpsNetwork>(clientSocket, ssl, context, timeoutInMilliseconds);
		}

		return streams::IOSocketStream::createStream<web::HttpNetwork>(clientSocket, timeoutInMilliseconds);
	}

	SSL* BaseWebServer::getNewSsl()
	{
		std::lock_guard<std::mutex> lock(sslMutex);

		return SSL_new(context);
	}

	BaseWebServer::BaseWebServer(WebFramework& frameworkInstance) :
		context(nullptr),
		certificate(nullptr),
		chain(nullptr),
		privateKey(nullptr),
		frameworkInstance(frameworkInstance)
	{
		this->updateCertificates();
	}

	void BaseWebServer::updateCertificates()
	{
		const std::optional<WebFramework::HttpsData>& httpsData = frameworkInstance.getHttpsData();

		if (!httpsData)
		{
			return;
		}

		BaseWebServer::readSsl
		(
			httpsData->getPathToCertificate(),
			[this](FILE* file, const std::filesystem::path& filePath)
			{
				if (certificate)
				{
					X509_free(certificate);
					sk_X509_pop_free(chain, X509_free);

					certificate = nullptr;
					chain = nullptr;
				}

				certificate = PEM_read_X509(file, nullptr, nullptr, nullptr);

				if (!certificate)
				{
					utility::logAndThrowException<logging::message::sslCertificateError, logging::category::https>(filePath.string());
				}

				chain = sk_X509_new_null();

				if (!chain)
				{
					throw std::bad_alloc();
				}

				while (true)
				{
					if (X509* additionalCert = PEM_read_X509(file, nullptr, nullptr, nullptr))
					{
						sk_X509_push(chain, additionalCert);
					}
					else
					{
						break;
					}
				}
			}
		);

		BaseWebServer::readSsl
		(
			httpsData->getPathToKey(),
			[this](FILE* file, const std::filesystem::path& filePath)
			{
				if (privateKey)
				{
					EVP_PKEY_free(privateKey);

					privateKey = nullptr;
				}

				privateKey = PEM_read_PrivateKey(file, nullptr, nullptr, nullptr);

				if (!privateKey)
				{
					utility::logAndThrowException<logging::message::privateKeyError, logging::category::https>(filePath.string());
				}
			}
		);

		if (!context)
		{
			context = SSL_CTX_new(TLS_server_method());
		}

		if (!context)
		{
			throw web::exceptions::SslException(__LINE__, __FILE__);
		}

		std::lock_guard<std::mutex> lock(sslMutex);

		if (int errorCode = SSL_CTX_use_certificate(context, certificate); errorCode != 1)
		{
			throw web::exceptions::SslException(__LINE__, __FILE__, errorCode);
		}

		for (int i = 0; i < sk_X509_num(chain); i++)
		{
			if (int errorCode = SSL_CTX_add1_chain_cert(context, sk_X509_value(chain, i)); errorCode != 1)
			{
				throw web::exceptions::SslException(__LINE__, __FILE__, errorCode);
			}
		}

		if (int errorCode = SSL_CTX_use_PrivateKey(context, privateKey); errorCode != 1)
		{
			throw web::exceptions::SslException(__LINE__, __FILE__, errorCode);
		}
	}

	WebFramework& BaseWebServer::getFrameworkInstance()
	{
		return frameworkInstance;
	}

	BaseWebServer::~BaseWebServer()
	{
		if (context)
		{
			SSL_CTX_free(context);
		}

		if (chain)
		{
			sk_X509_pop_free(chain, X509_free);
		}

		if (certificate)
		{
			X509_free(certificate);
		}
		
		if (privateKey)
		{
			EVP_PKEY_free(privateKey);
		}
	}
}
