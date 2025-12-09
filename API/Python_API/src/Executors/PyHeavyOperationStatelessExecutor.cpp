#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "PyHeavyOperationStatelessExecutor.h"

#include <pybind11/pybind11.h>

namespace framework
{
	void PyHeavyOperationStatelessExecutor::init(const utility::ExecutorSettings& settings)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatelessExecutor,
			init,
			settings
		);
	}

	void PyHeavyOperationStatelessExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatelessExecutor,
			doPost,
			request,
			response
		);
	}

	void PyHeavyOperationStatelessExecutor::doGet(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatelessExecutor,
			doGet,
			request,
			response
		);
	}

	void PyHeavyOperationStatelessExecutor::doHead(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatelessExecutor,
			doHead,
			request,
			response
		);
	}

	void PyHeavyOperationStatelessExecutor::doPut(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatelessExecutor,
			doPut,
			request,
			response
		);
	}

	void PyHeavyOperationStatelessExecutor::doDelete(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatelessExecutor,
			doDelete,
			request,
			response
		);
	}

	void PyHeavyOperationStatelessExecutor::doPatch(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatelessExecutor,
			doPatch,
			request,
			response
		);
	}

	void PyHeavyOperationStatelessExecutor::doOptions(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatelessExecutor,
			doOptions,
			request,
			response
		);
	}

	void PyHeavyOperationStatelessExecutor::doTrace(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatelessExecutor,
			doTrace,
			request,
			response
		);
	}

	void PyHeavyOperationStatelessExecutor::doConnect(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatelessExecutor,
			doConnect,
			request,
			response
		);
	}
}
