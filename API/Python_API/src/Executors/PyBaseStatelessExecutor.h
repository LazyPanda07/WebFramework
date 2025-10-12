#pragma once

#include <Executors/BaseStatelessExecutor.hpp>

namespace framework
{
	class PyBaseStatelessExecutor : public BaseStatelessExecutor
	{
	public:
		PyBaseStatelessExecutor() = default;

		void init(const utility::ExecutorSettings& settings) override;

		void doPost(HTTPRequest& request, HTTPResponse& response) override;

		void doGet(HTTPRequest& request, HTTPResponse& response) override;

		void doHead(HTTPRequest& request, HTTPResponse& response) override;

		void doPut(HTTPRequest& request, HTTPResponse& response) override;

		void doDelete(HTTPRequest& request, HTTPResponse& response) override;

		void doPatch(HTTPRequest& request, HTTPResponse& response) override;

		void doOptions(HTTPRequest& request, HTTPResponse& response) override;

		void doTrace(HTTPRequest& request, HTTPResponse& response) override;

		void doConnect(HTTPRequest& request, HTTPResponse& response) override;

		~PyBaseStatelessExecutor() = default;
	};
}
