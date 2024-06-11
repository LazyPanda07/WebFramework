#pragma once

#include <memory>
#include <format>

#include "DLLHandler.h"
#include "WebFramework.h"

namespace framework_api
{
	class Service
	{
	private:
		std::shared_ptr<DLLHandler> handler;

	private:
		Service(const std::filesystem::path& pathToDLL);

		~Service() = default;

	public:
		static Service& createService(const std::filesystem::path& pathToDLL);

	public:
		WebFramework createWebFramework(const std::filesystem::path& configPath);

		WebFramework createWebFramework(std::string_view serverConfiguration, std::string_view sourcesPath);

	public:
		Service() = delete;

		Service(const Service&) = delete;

		Service(Service&&) noexcept = delete;

		Service& operator = (const Service&) = delete;

		Service& operator = (Service&&) noexcept = delete;

		friend struct std::default_delete<Service>;
	};
}

namespace framework_api
{
	Service::Service(const std::filesystem::path& pathToDLL)
	{
		auto makePathToDLL = [](const std::filesystem::path& pathToSource) -> std::filesystem::path
			{
#ifdef __LINUX__
				filesystem::path parent = pathToSource.parent_path();
				filesystem::path fileName = pathToSource.filename();

				return std::format("{}/lib{}.so", parent.string(), fileName.string());
#else
				return std::format("{}.dll", pathToSource.string());
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
			throw std::runtime_error(std::format("Can't find {}", configPath.string()));
		}

		return WebFramework(handler, configPath.string());
	}

	inline WebFramework Service::createWebFramework(std::string_view serverConfiguration, std::string_view sourcesPath)
	{
		return WebFramework(handler, serverConfiguration, sourcesPath);
	}

	inline Service& Service::createService(const std::filesystem::path& pathToDLL)
	{
		static std::unique_ptr<Service> instance;

		if (!instance)
		{
			instance = std::unique_ptr<Service>(new Service(pathToDLL));
		}

		return *instance;
	}
}
