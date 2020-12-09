#pragma once

#include <filesystem>

#include "BaseExecutor.h"

namespace framework
{
	class ResourceExecutor : public BaseExecutor
	{
	private:
		std::filesystem::path defaultAssets;
		std::filesystem::path assets;

	private:
		void sendAssetFile(HTTPRequest&& request, HTTPResponse& response);

	public:
		ResourceExecutor(const std::filesystem::path& assets);

		void init(const utility::XMLSettingsParser::ExecutorSettings& settings) override;

		void doGet(HTTPRequest&& request, HTTPResponse& response) override;

		void doPost(HTTPRequest&& request, HTTPResponse& response) override;

		void destroy() override;

		~ResourceExecutor() = default;
	};
}