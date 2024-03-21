#pragma once

#include "headers.h"

#include "BaseWebServer.h"

namespace framework
{
	class ThreadPoolWebServer : public virtual BaseWebServer
	{
	private:
		threading::ThreadPool threadPool;

	private:
		void taskImplementation(streams::IOSocketStream&& stream, HTTPRequest&& request, std::function<void(HTTPRequest&, HTTPResponse&)> executorMethod);

		void clientConnection(const std::string& ip, SOCKET clientSocket, const sockaddr& addr, std::function<void()>&& cleanup) override;

	public:
		ThreadPoolWebServer(const json::JSONParser& configuration, const std::vector<utility::JSONSettingsParser>& parsers, const std::filesystem::path& assets, const std::string& pathToTemplates, uint64_t cachingSize, const std::string& ip, const std::string& port, DWORD timeout, const std::vector<std::string>& pathToSources, uint32_t threadCount);

		~ThreadPoolWebServer() = default;
	};
}
