#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "PyStatelessExecutor.h"

#include <pybind11/pybind11.h>

namespace framework
{
	void PyStatelessExecutor::init(const utility::ExecutorSettings& settings)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatelessExecutor,
			init,
			settings
		);
	}

	void PyStatelessExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatelessExecutor,
			doPost,
			request,
			response
		);
	}

	void PyStatelessExecutor::doGet(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatelessExecutor,
			doGet,
			request,
			response
		);
	}

	void PyStatelessExecutor::doHead(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatelessExecutor,
			doHead,
			request,
			response
		);
	}

	void PyStatelessExecutor::doPut(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatelessExecutor,
			doPut,
			request,
			response
		);
	}

	void PyStatelessExecutor::doDelete(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatelessExecutor,
			doDelete,
			request,
			response
		);
	}

	void PyStatelessExecutor::doPatch(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatelessExecutor,
			doPatch,
			request,
			response
		);
	}

	void PyStatelessExecutor::doOptions(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatelessExecutor,
			doOptions,
			request,
			response
		);
	}

	void PyStatelessExecutor::doTrace(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatelessExecutor,
			doTrace,
			request,
			response
		);
	}

	void PyStatelessExecutor::doConnect(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			StatelessExecutor,
			doConnect,
			request,
			response
		);
	}
}
