#pragma once

#include <filesystem>

#include "BaseStatelessExecutor.h"

namespace framework
{
	class ResourceExecutor : public BaseStatelessExecutor
	{
	private:
		const std::filesystem::path defaultAssets;
		const std::filesystem::path assets;
		std::unordered_map<std::string, std::string> cache;
		bool isCaching;

	private:
		void sendAssetFile(HTTPRequest&& request, HTTPResponse& response);

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