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
			BaseHeavyOperationStatelessExecutor,
			init,
			settings
		);
	}

	void PyHeavyOperationStatelessExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatelessExecutor,
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
			BaseHeavyOperationStatelessExecutor,
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
			BaseHeavyOperationStatelessExecutor,
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
			BaseHeavyOperationStatelessExecutor,
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
			BaseHeavyOperationStatelessExecutor,
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
			BaseHeavyOperationStatelessExecutor,
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
			BaseHeavyOperationStatelessExecutor,
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
			BaseHeavyOperationStatelessExecutor,
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
			BaseHeavyOperationStatelessExecutor,
			doConnect,
			request,
			response
		);
	}
}
