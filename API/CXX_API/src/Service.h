#pragma once

#include <memory>
#include <format>

#include "DLLHandler.h"
#include "WebFramework.h"
#include "Config.h"

namespace framework
{
	class Service
	{
	private:
		std::shared_ptr<DLLHandler> handler;

	private:
		Service(const std::filesystem::path& pathToDLL);

		~Service() = default;

	public:
		static Service& createService(const std::filesystem::path& pathToDLL = "WebFramework");

	public:
		WebFramework createWebFramework(const std::filesystem::path& configPath);

		WebFramework createWebFramework(std::string_view serverConfiguration, std::string_view sourcesPath);

		WebFramework createWebFramework(const utility::Config& config);

		utility::Config createConfig(const std::filesystem::path& configPath);

		utility::Config createConfig(std::string_view serverConfiguration, std::string_view sourcesPath);

	public:
		Service() = delete;

		Service(const Service&) = delete;

		Service(Service&&) noexcept = delete;

		Service& operator = (const Service&) = delete;

		Service& operator = (Service&&) noexcept = delete;

		friend struct std::default_delete<Service>;
	};
}

namespace framework
{
	inline Service::Service(const std::filesystem::path& pathToDLL)
	{
		auto makePathToDLL = [](const std::filesystem::path& pathToSource) -> std::filesystem::path
			{
#ifndef __LINUX__
				return std::format("{}.dll", pathToSource.string());

#else
				std::filesystem::path parent = pathToSource.parent_path();
				std::filesystem::path fileName = pathToSource.filename();

				return std::format("{}/lib{}.so", parent.string(), fileName.string());
#endif
			};

		std::filesystem::path realPath = makePathToDLL(pathToDLL);

		if (!std::filesystem::exists(realPath))
		{
			throw std::runtime_error(std::format("Path {} doesn't exist", realPath.string()));
		}

		handler = std::make_shared<DLLHandler>(realPath);
	}

	inline WebFramework Service::createWebFramework(const std::filesystem::path& configPath)
	{
		if (!std::filesystem::exists(configPath))
		{
			throw std::runtime_error(std::format("Path {} doesn't exist", configPath.string()));
		}

		return WebFramework(handler, configPath.string());
	}

	inline WebFramework Service::createWebFramework(std::string_view serverConfiguration, std::string_view sourcesPath)
	{
		return WebFramework(handler, serverConfiguration, sourcesPath);
	}

	inline WebFramework Service::createWebFramework(const utility::Config& config)
	{
		return WebFramework(handler, config);
	}

	inline utility::Config Service::createConfig(const std::filesystem::path& configPath)
	{
		if (!std::filesystem::exists(configPath))
		{
			throw std::runtime_error(std::format("Path {} doesn't exist", configPath.string()));
		}

		return utility::Config(handler, configPath);
	}

	inline utility::Config Service::createConfig(std::string_view serverConfiguration, std::string_view sourcesPath)
	{
		return utility::Config(handler, serverConfiguration, sourcesPath);
	}

	inline Service& Service::createService(const std::filesystem::path& pathToDLL)
	{
		static std::unique_ptr<Service> instance;

		if (!instance)
		{
			instance = std::unique_ptr<Service>(new Service(std::filesystem::absolute(pathToDLL)));
		}

		return *instance;
	}
}
