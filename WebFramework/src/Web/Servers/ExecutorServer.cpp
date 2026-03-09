#include "Web/Servers/ExecutorServer.h"

#include <cstring>

#include <Log.h>
#include <Exceptions/FileDoesNotExistException.h>

#include "Exceptions/NotFoundException.h"
#include "Exceptions/APIException.h"
#include "Exceptions/CSharpException.h"

namespace framework
{
	ExecutorServer::ServiceState ExecutorServer::serviceRequests(streams::IOSocketStream& stream, HttpRequestImplementation& request, HttpResponseImplementation& response, ResourceExecutor& resources, const std::function<void(ServiceState&)>& task)
	{
		ServiceState result = ServiceState::success;

		try
		{
			task(result);
		}
		catch (const web::exceptions::WebException& e)
		{
			if (Log::isValid())
			{
				Log::error("Executors serve exception: {}", "LogExecutorServer", e.what());
			}

			result = ServiceState::error;
		}
#ifdef __WITH_DOTNET_EXECUTORS__
		catch (const exceptions::CSharpException& e)
		{
			if (Log::isValid())
			{
				Log::error("Exception from API: {} with response code: {}", e.getLogCategory(), e.what(), static_cast<int64_t>(e.getResponseCode()));
			}

			const char* exceptionMessage = e.what();

			response.setResponseCode(static_cast<int64_t>(e.getResponseCode()));
			response.setBody(exceptionMessage, std::strlen(exceptionMessage));
		}
#endif
		catch (const exceptions::BadRequestException& e) // 400
		{
			if (Log::isValid())
			{
				Log::warning("Bad request from client: {}", "LogExecutorServer", e.what());
			}

			resources.badRequestError(response, &e);

			stream << response;

			result = ServiceState::skipResponse;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException& e) // 404
		{
			if (Log::isValid())
			{
				Log::warning("Can't find file exception: {}", "LogExecutorServer", e.what());
			}

			resources.notFoundError(response, &e);

			stream << response;

			result = ServiceState::skipResponse;
		}
		catch (const exceptions::NotFoundException& e) // 404
		{
			// TODO: add throwing exception for some reason, logging

			resources.notFoundError(response, &e);

			stream << response;

			result = ServiceState::skipResponse;
		}
		catch (const exceptions::APIException& e)
		{
			if (Log::isValid())
			{
				Log::error("Exception from API: {} with response code: {}", e.getLogCategory(), e.what(), e.getResponseCode());
			}

			const char* exceptionMessage = e.what();

			response.setResponseCode(e.getResponseCode());
			response.setBody(exceptionMessage, std::strlen(exceptionMessage));

			stream << response;

			result = ServiceState::skipResponse;
		}
		catch (const exceptions::ExecutorException& e) // 500
		{
			if (Log::isValid())
			{
				Log::error("Executor internal server error: {}", "LogExecutorServer", e.what());
			}

			resources.internalServerError(response, &e);

			stream << response;

			result = ServiceState::skipResponse;
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
				response.setBody(exceptionData.errorMessage, std::strlen(exceptionData.errorMessage));
			}
			else
			{
				if (Log::isValid())
				{
					Log::error("Internal server error: {}", "LogExecutorServer", e.what());
				}

				resources.internalServerError(response, &e);
			}

			stream << response;

			result = ServiceState::skipResponse;
		}
		catch (...)	// 500
		{
			resources.internalServerError(response, nullptr);

			stream << response;

			result = ServiceState::skipResponse;
		}

		return result;
	}

	ExecutorServer::ExecutorServer
	(
		const json::JsonParser& configuration,
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		const std::vector<std::string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings,
		std::shared_ptr<threading::ThreadPool> threadPool
	) :
		additionalSettings(additionalSettings)
	{
		executorsManager = std::make_unique<ExecutorsManager>(configuration, pathToSources, std::move(executorsSettings), additionalSettings, threadPool);

		resources = executorsManager->getResourceExecutor();
	}
}
