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
			BaseStatefulExecutor,
			init,
			settings
		);
	}

	void PyStatefulExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			BaseStatefulExecutor,
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
			BaseStatefulExecutor,
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
			BaseStatefulExecutor,
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
			BaseStatefulExecutor,
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
			BaseStatefulExecutor,
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
			BaseStatefulExecutor,
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
			BaseStatefulExecutor,
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
			BaseStatefulExecutor,
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
			BaseStatefulExecutor,
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
			BaseStatefulExecutor,
			destroy
		);
	}
}
