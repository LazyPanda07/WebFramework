#include "Web/Servers/BaseWebServer.h"

#include <Exceptions/SslException.h>
#include <Exceptions/FileDoesNotExistException.h>
#include <HttpsNetwork.h>

#include "Utility/Singletons/HTTPSSingleton.h"

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

	BaseWebServer::BaseWebServer() :
		context(nullptr),
		certificate(nullptr),
		privateKey(nullptr)
	{
		this->updateCertificates();
	}

	void BaseWebServer::updateCertificates()
	{
		utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();

		if (useHTTPS = httpsSettings.getUseHTTPS(); useHTTPS)
		{
			BaseWebServer::readSsl
			(
				httpsSettings.getPathToCertificate(),
				[this](FILE* file, const std::filesystem::path& filePath)
				{
					if (certificate)
					{
						X509_free(certificate);
					}

					certificate = PEM_read_X509(file, nullptr, nullptr, nullptr);

					if (!certificate)
					{
						throw std::runtime_error(std::format("Error while reading certificate from {}", filePath.string()));
					}
				}
			);

			BaseWebServer::readSsl
			(
				httpsSettings.getPathToKey(),
				[this](FILE* file, const std::filesystem::path& filePath)
				{
					if (privateKey)
					{
						EVP_PKEY_free(privateKey);
					}

					privateKey = PEM_read_PrivateKey(file, nullptr, nullptr, nullptr);

					if (!privateKey)
					{
						throw std::runtime_error(std::format("Error while reading private key from {}", filePath.string()));
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

			if (int errorCode = SSL_CTX_use_PrivateKey(context, privateKey); errorCode != 1)
			{
				throw web::exceptions::SslException(__LINE__, __FILE__, errorCode);
			}
		}
	}

	BaseWebServer::~BaseWebServer()
	{
		if (useHTTPS)
		{
			SSL_CTX_free(context);
			X509_free(certificate);
			EVP_PKEY_free(privateKey);
		}
	}
}
