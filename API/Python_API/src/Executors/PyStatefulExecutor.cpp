#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "PyStatefulExecutor.h"

#include <pybind11/pybind11.h>

namespace framework
{
	void PyStatefulExecutor::init(const utility::ExecutorSettings& settings)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			init,
			settings
		);
	}

	void PyStatefulExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			doPost,
			request,
			response
		);
	}

	void PyStatefulExecutor::doGet(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			doGet,
			request,
			response
		);
	}

	void PyStatefulExecutor::doHead(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			doHead,
			request,
			response
		);
	}

	void PyStatefulExecutor::doPut(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			doPut,
			request,
			response
		);
	}

	void PyStatefulExecutor::doDelete(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			doDelete,
			request,
			response
		);
	}

	void PyStatefulExecutor::doPatch(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			doPatch,
			request,
			response
		);
	}

	void PyStatefulExecutor::doOptions(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			doOptions,
			request,
			response
		);
	}

	void PyStatefulExecutor::doTrace(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			doTrace,
			request,
			response
		);
	}

	void PyStatefulExecutor::doConnect(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			doConnect,
			request,
			response
		);
	}

	void PyStatefulExecutor::destroy()
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatefulExecutor,
			destroy
		);
	}
}
