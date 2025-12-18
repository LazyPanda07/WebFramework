#pragma once

#include "Runtime.h"

#include <Strings.h>

namespace framework::runtime
{
	class CXXRuntime : public Runtime
	{
	private:
		::utility::strings::string_based_unordered_map<std::tuple<HMODULE, CreateExecutorSignature>> creators;

	public:
		CXXRuntime() = default;

		void finishInitialization() override;

		bool loadExecutor(std::string_view name, const utility::LoadSource& source) override;

		std::unique_ptr<BaseExecutor> createExecutor(std::string_view name) const override;

		void* createExecutorSettings(const void* implementation) const override;

		void* createHTTPRequest(framework::interfaces::IHTTPRequest* request) const override;

		void* createHTTPResponse(framework::interfaces::IHTTPResponse* response) const override;

		void initializeWebFramework(const utility::LoadSource& source, std::string_view libraryPath) override;

		std::optional<std::string> loadSource(std::string_view pathToSource, utility::LoadSource& source) override;

		~CXXRuntime() = default;
	};
}
