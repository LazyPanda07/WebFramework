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
			HeavyOperationStatefulExecutor,
			init,
			settings
		);
	}

	void PyHeavyOperationStatefulExecutor::doPost(HttpRequest& request, HttpResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatefulExecutor,
			doPost,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doGet(HttpRequest& request, HttpResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatefulExecutor,
			doGet,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doHead(HttpRequest& request, HttpResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatefulExecutor,
			doHead,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doPut(HttpRequest& request, HttpResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatefulExecutor,
			doPut,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doDelete(HttpRequest& request, HttpResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatefulExecutor,
			doDelete,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doPatch(HttpRequest& request, HttpResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatefulExecutor,
			doPatch,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doOptions(HttpRequest& request, HttpResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatefulExecutor,
			doOptions,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doTrace(HttpRequest& request, HttpResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatefulExecutor,
			doTrace,
			request,
			response
		);
	}

	void PyHeavyOperationStatefulExecutor::doConnect(HttpRequest& request, HttpResponse& response)
	{
		PYBIND11_OVERRIDE
		(
			void,
			HeavyOperationStatefulExecutor,
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
			HeavyOperationStatefulExecutor,
			destroy
		);
	}
}
