#pragma once

#include <Executors/BaseExecutor.h>

#ifdef __WITH_DOT_NET_EXECUTORS__

#include "Runtimes/DotNetRuntime.h"

namespace framework
{
	class DotNetExecutor : public BaseExecutor
	{
	private:
		void* implementation;
		runtime::DotNetRuntime::NativeString moduleName;

	public:
		DotNetExecutor(void* implementation, const std::filesystem::path& modulePath);

		void init(const utility::JSONSettingsParser::ExecutorSettings& settings) override;

		void doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		utility::ExecutorType getType() const override;

		void destroy() override;

		~DotNetExecutor();
	};
}

#endif
