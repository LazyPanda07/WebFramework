#pragma once

#include <filesystem>
#include <shared_mutex>

#include "BaseStatelessExecutor.h"
#include "WebNetwork/Interfaces/ISendFile.h"

namespace framework
{
	class ResourceExecutor : interfaces::ISendFile, public BaseStatelessExecutor
	{
	private:
		std::shared_mutex cacheMutex;
		const std::filesystem::path defaultAssets;
		const std::filesystem::path assets;
		std::unordered_map<std::string, std::string> cache;
		bool isCaching;

	private:
		void sendFile(const std::string& filePath, HTTPResponse& response) override;

	public:
		ResourceExecutor(const std::filesystem::path& assets, bool isCaching);

		void init(const utility::XMLSettingsParser::ExecutorSettings& settings) override;

		void doGet(HTTPRequest&& request, HTTPResponse& response) override;

		void doPost(HTTPRequest&& request, HTTPResponse& response) override;

		void notFoundError(HTTPResponse& response);

		void badRequestError(HTTPResponse& response);

		void internalServerError(HTTPResponse& response);

		void destroy() override;

		~ResourceExecutor() = default;
	};
}