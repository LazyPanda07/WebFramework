#include "ExecutorServer.h"

#include <Log.h>
#include <Exceptions/FileDoesNotExistException.h>

#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/NotFoundException.h"
#include "Exceptions/APIException.h"
#include "Utility/Sources.h"
#include "Utility/DynamicLibraries.h"

namespace framework
{
	bool ExecutorServer::serviceRequests(streams::IOSocketStream& stream, HTTPRequestImplementation& request, HTTPResponseImplementation& response, ExecutorsManager::StatefulExecutors& executors, web::LargeBodyHandler* largeBodyHandler)
	{
		HTTPResponseExecutors responseWrapper(&response);

		try
		{
			stream >> request;

			if (largeBodyHandler && largeBodyHandler->isRunning())
			{
				return true;
			}

			if (stream.eof())
			{
				return false;
			}

			HTTPRequestExecutors requestWrapper(&request);

			executorsManager->service(requestWrapper, responseWrapper, executors);

			if (response)
			{
				stream << response;
			}

			if (stream.eof())
			{
				return false;
			}
		}
		catch (const web::exceptions::WebException& e)
		{
			if (Log::isValid())
			{
				Log::error("Executors serve exception: {}", "LogExecutorServer", e.what());
			}

			return false;
		}
		catch (const exceptions::BadRequestException& e) // 400
		{
			resources->badRequestError(responseWrapper, &e);

			stream << response;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException& e) // 404
		{
			resources->notFoundError(responseWrapper, &e);

			stream << response;
		}
		catch (const exceptions::NotFoundException& e) // 404
		{
			resources->notFoundError(responseWrapper, &e);

			stream << response;
		}
		catch (const exceptions::APIException& e)
		{
			if (Log::isValid())
			{
				Log::error("Exception from API: {} with response code: {}", e.getLogCategory(), e.what(), e.getResponseCode());
			}

			response.setResponseCode(e.getResponseCode());
			response.setBody(e.what());

			stream << response;
		}
		catch (const exceptions::BaseExecutorException& e) // 500
		{
			if (Log::isValid())
			{
				Log::error("Internal server error: {}", "LogExecutorServer", e.what());
			}

			resources->internalServerError(responseWrapper, &e);

			stream << response;
		}
		catch (const std::exception& e)
		{
			framework::interfaces::CExceptionData exceptionData;

			if (request.getExceptionData(&exceptionData))
			{
				if (Log::isValid())
				{
					Log::error("Exception from API: {} with response code: {}", exceptionData.logCategory, e.what(), exceptionData.responseCode);
				}

				response.setResponseCode(exceptionData.responseCode);
				response.setBody(exceptionData.errorMessage);
			}
			else
			{
				if (Log::isValid())
				{
					Log::error("Internal server error: {}", "LogExecutorServer", e.what());
				}

				resources->internalServerError(responseWrapper, &e);
			}

			stream << response;
		}
		catch (...)	// 500
		{
			resources->internalServerError(responseWrapper, nullptr);

			stream << response;
		}

		return true;
	}

	ExecutorServer::ExecutorServer
	(
		const json::JSONParser& configuration,
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		const std::vector<std::string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings,
		std::shared_ptr<threading::ThreadPool> threadPool
	) :
		additionalSettings(additionalSettings)
	{
		try
		{
			executorsManager = make_unique<ExecutorsManager>(configuration, pathToSources, move(executorsSettings), additionalSettings, threadPool);

			resources = executorsManager->getResourceExecutor();
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::fatalError("Can't create server: {}", "LogExecutorServer", 2, e.what());
			}

			throw;
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::fatalError("Something went wrong", "LogExecutorServer", 2);
			}

			throw;
		}
	}
}
