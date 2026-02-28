#pragma once

#include "Runtime.h"

#include <Strings.h>

namespace framework::runtime
{
	class CXXRuntime : public Runtime
	{
	public:
		static constexpr std::string_view runtimeName = "cxx";

	private:
		::utility::strings::string_based_unordered_map<std::tuple<HMODULE, CreateExecutorSignature>> creators;

	public:
		CXXRuntime() = default;

		void finishInitialization() override;

		bool loadExecutor(std::string_view name, std::string_view route, const utility::LoadSource& source) override;

		std::unique_ptr<Executor> createExecutor(std::string_view name) const override;

		void* createExecutorSettings(const void* implementation) const override;

		void* createHTTPRequest(framework::interfaces::IHttpRequest* request) const override;

		void* createHTTPResponse(framework::interfaces::IHttpResponse* response) const override;

		void initializeWebFramework(const utility::LoadSource& source, std::string_view libraryPath) override;

		std::optional<std::string> loadSource(std::string_view pathToSource, utility::LoadSource& source) override;

		constexpr std::string_view getName() const override;

		~CXXRuntime() = default;
	};
}

namespace framework::runtime
{
	inline constexpr std::string_view CXXRuntime::getName() const
	{
		return CXXRuntime::runtimeName;
	}
}
