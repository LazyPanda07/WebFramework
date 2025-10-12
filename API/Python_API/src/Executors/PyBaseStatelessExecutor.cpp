#include "PyBaseStatelessExecutor.h"

#include <pybind11/pybind11.h>

namespace framework
{
	void PyBaseStatelessExecutor::init(const utility::ExecutorSettings& settings)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseStatelessExecutor,
			init,
			settings
		);
	}

	void PyBaseStatelessExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			BaseStatelessExecutor,
			doPost,
			request,
			response
		);
	}

	void PyBaseStatelessExecutor::doGet(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			BaseStatelessExecutor,
			doGet,
			request,
			response
		);
	}

	void PyBaseStatelessExecutor::doHead(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			BaseStatelessExecutor,
			doHead,
			request,
			response
		);
	}

	void PyBaseStatelessExecutor::doPut(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			BaseStatelessExecutor,
			doPut,
			request,
			response
		);
	}

	void PyBaseStatelessExecutor::doDelete(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			BaseStatelessExecutor,
			doDelete,
			request,
			response
		);
	}

	void PyBaseStatelessExecutor::doPatch(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			BaseStatelessExecutor,
			doPatch,
			request,
			response
		);
	}

	void PyBaseStatelessExecutor::doOptions(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			BaseStatelessExecutor,
			doOptions,
			request,
			response
		);
	}

	void PyBaseStatelessExecutor::doTrace(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			BaseStatelessExecutor,
			doTrace,
			request,
			response
		);
	}

	void PyBaseStatelessExecutor::doConnect(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE_PURE
		(
			void,
			BaseStatelessExecutor,
			doConnect,
			request,
			response
		);
	}
}
