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

	void PyHeavyOperationStatelessExecutor::doPost(HttpRequest& request, HttpResponse& response)
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

	void PyHeavyOperationStatelessExecutor::doGet(HttpRequest& request, HttpResponse& response)
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

	void PyHeavyOperationStatelessExecutor::doHead(HttpRequest& request, HttpResponse& response)
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

	void PyHeavyOperationStatelessExecutor::doPut(HttpRequest& request, HttpResponse& response)
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

	void PyHeavyOperationStatelessExecutor::doDelete(HttpRequest& request, HttpResponse& response)
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

	void PyHeavyOperationStatelessExecutor::doPatch(HttpRequest& request, HttpResponse& response)
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

	void PyHeavyOperationStatelessExecutor::doOptions(HttpRequest& request, HttpResponse& response)
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

	void PyHeavyOperationStatelessExecutor::doTrace(HttpRequest& request, HttpResponse& response)
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

	void PyHeavyOperationStatelessExecutor::doConnect(HttpRequest& request, HttpResponse& response)
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
