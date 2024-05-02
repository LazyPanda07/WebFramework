#include "CRUDExecutor.h"

void CRUDExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
{

}

void CRUDExecutor::doPost(framework::HTTPRequest& request, framework::HTTPResponse& response)
{
	request.getDatabaseModelInstance();
}

void CRUDExecutor::doPut(framework::HTTPRequest& request, framework::HTTPResponse& response)
{

}

void CRUDExecutor::destroy()
{

}

DECLARE_EXECUTOR(CRUDExecutor)
