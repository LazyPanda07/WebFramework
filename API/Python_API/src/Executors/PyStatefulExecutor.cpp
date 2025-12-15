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

	void PyStatefulExecutor::doPost(HttpRequest& request, HttpResponse& response)
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

	void PyStatefulExecutor::doGet(HttpRequest& request, HttpResponse& response)
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

	void PyStatefulExecutor::doHead(HttpRequest& request, HttpResponse& response)
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

	void PyStatefulExecutor::doPut(HttpRequest& request, HttpResponse& response)
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

	void PyStatefulExecutor::doDelete(HttpRequest& request, HttpResponse& response)
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

	void PyStatefulExecutor::doPatch(HttpRequest& request, HttpResponse& response)
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

	void PyStatefulExecutor::doOptions(HttpRequest& request, HttpResponse& response)
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

	void PyStatefulExecutor::doTrace(HttpRequest& request, HttpResponse& response)
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

	void PyStatefulExecutor::doConnect(HttpRequest& request, HttpResponse& response)
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
