#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "PyHeavyOperationStatefulExecutor.h"

#include <pybind11/pybind11.h>

namespace framework
{
	void PyHeavyOperationStatefulExecutor::init(const utility::ExecutorSettings& settings)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			init,
			settings
		);
	}

	void PyHeavyOperationStatefulExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			doPost,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doGet(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			doGet,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doHead(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			doHead,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doPut(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			doPut,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doDelete(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			doDelete,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doPatch(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			doPatch,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doOptions(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			doOptions,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doTrace(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			doTrace,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doConnect(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			doConnect,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::destroy()
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseHeavyOperationStatefulExecutor,
			destroy
		);
	}
}
