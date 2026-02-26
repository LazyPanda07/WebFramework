#pragma once

#include "BaseTCPServer.h"

#include <filesystem>
#include <mutex>

#include <openssl/ssl.h>

#include <IOSocketStream.h>

#include "Framework/WebFrameworkPlatform.h"
#include "Utility/JSONSettingsParser.h"

namespace framework
{
	class BaseWebServer : public virtual web::BaseTCPServer
	{
	private:
		std::mutex sslMutex;
		SSL_CTX* context;
		X509* certificate;
		EVP_PKEY* privateKey;

	protected:
		bool useHTTPS;

	private:
		static void readSsl(const std::filesystem::path& filePath, const std::function<void(FILE*, const std::filesystem::path&)>& callback);

	private:
		bool autoCloseSocket() const final override;

	protected:
		streams::IOSocketStream createServerSideStream(SOCKET clientSocket, SSL* ssl, std::chrono::milliseconds timeoutInMilliseconds);

		SSL* getNewSsl();

	public:
		BaseWebServer();

		void updateCertificates();

		virtual ~BaseWebServer();
	};
}
