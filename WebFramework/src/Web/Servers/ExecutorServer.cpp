#include "Web/Servers/ExecutorServer.h"

#include <cstring>

#include <Exceptions/FileDoesNotExistException.h>

#include "Exceptions/NotFoundException.h"
#include "Exceptions/APIException.h"
#include "Exceptions/BadRequestException.h"
#include "Exceptions/CSharpException.h"
#include "Exceptions/ForbiddenException.h"
#include "Utility/Utils.h"

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
				Log::error<logging::message::executorsServeException, logging::category::executorServer>(e.what());
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

			utility::processStreamOperation<logging::category::executorServer, utility::structs::SendOperation>(stream, response);

			result = ServiceState::skipResponse;
		}
#endif
		catch (const exceptions::BadRequestException& e) // 400
		{
			if (Log::isValid())
			{
				Log::error<logging::message::badRequest, logging::category::executorServer>(e.what());
			}

			resources.badRequestError(response, &e);

			utility::processStreamOperation<logging::category::executorServer, utility::structs::SendOperation>(stream, response);

			result = ServiceState::skipResponse;
		}
		catch (const framework::exceptions::ForbiddenException& e) // 403
		{
			if (Log::isValid())
			{
				Log::error<logging::message::forbiddenMessage, logging::category::executorServer>(e.what());
			}

			resources.forbiddenError(response, &e);

			utility::processStreamOperation<logging::category::executorServer, utility::structs::SendOperation>(stream, response);

			result = ServiceState::skipResponse;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException& e) // 404
		{
			if (Log::isValid())
			{
				Log::error<logging::message::cantFindFile, logging::category::executorServer>(e.what());
			}

			resources.notFoundError(response, &e);

			utility::processStreamOperation<logging::category::executorServer, utility::structs::SendOperation>(stream, response);

			result = ServiceState::skipResponse;
		}
		catch (const exceptions::NotFoundException& e) // 404
		{
			if (Log::isValid())
			{
				Log::error<logging::message::notFound, logging::category::executorServer>(e.what());
			}

			resources.notFoundError(response, &e);

			utility::processStreamOperation<logging::category::executorServer, utility::structs::SendOperation>(stream, response);

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

			utility::processStreamOperation<logging::category::executorServer, utility::structs::SendOperation>(stream, response);

			result = ServiceState::skipResponse;
		}
		catch (const exceptions::ExecutorException& e) // 500
		{
			if (Log::isValid())
			{
				Log::error<logging::message::executorInternalServer, logging::category::executorServer>(e.what());
			}

			resources.internalServerError(response, &e);

			utility::processStreamOperation<logging::category::executorServer, utility::structs::SendOperation>(stream, response);

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
					Log::error<logging::message::internalServerError, logging::category::executorServer>(e.what());
				}

				resources.internalServerError(response, &e);
			}

			utility::processStreamOperation<logging::category::executorServer, utility::structs::SendOperation>(stream, response);

			result = ServiceState::skipResponse;
		}
		catch (...)	// 500
		{
			resources.internalServerError(response, nullptr);

			utility::processStreamOperation<logging::category::executorServer, utility::structs::SendOperation>(stream, response);

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
