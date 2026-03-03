#pragma once

#include "Runtime.h"

#include <Strings.h>

namespace framework::runtime
{
	class CCRuntime : public Runtime
	{
	public:
		static constexpr std::string_view runtimeName = "cc";

	protected:
		void initializeWebFramework(const utility::LoadSource& source, std::string_view libraryPath) override;

	private:
		::utility::strings::string_based_unordered_map<std::tuple<HMODULE, CreateExecutorSignature>> creators;

	public:
		CCRuntime() = default;

		void finishInitialization() override;

		bool loadExecutor(std::string_view name, std::string_view route, const utility::LoadSource& source) override;

		std::unique_ptr<Executor> createExecutor(std::string_view name) const override;

		std::unique_ptr<task_broker::TaskExecutor> createTaskExecutor(std::string_view name, const utility::LoadSource& source) const override;

		void* createExecutorSettings(const void* implementation) const override;

		void* createHTTPRequest(framework::interfaces::IHttpRequest* request) const override;

		void* createHTTPResponse(framework::interfaces::IHttpResponse* response) const override;

		std::optional<std::string> loadSource(std::string_view pathToSource, utility::LoadSource& source) override;

		constexpr std::string_view getName() const override;

		~CCRuntime() = default;
	};
}

namespace framework::runtime
{
	inline constexpr std::string_view CCRuntime::getName() const
	{
		return CCRuntime::runtimeName;
	}
}
