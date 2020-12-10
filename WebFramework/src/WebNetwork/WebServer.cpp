#include "WebServer.h"

#include "HTTPNetwork.h"
#include "BaseIOSocketStream.h"
#include "Exceptions/NotImplementedException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"

#pragma warning(push)
#pragma warning(disable: 6387)

using namespace std;

namespace framework
{
	void WebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream stream(new buffers::IOSocketBuffer(new HTTPNetwork(clientSocket)));
		const string clientIp = getIpV4(addr);
		unique_ptr<ResourceExecutor>& resources = executorsManager.getResourceExecutor();
		unordered_map<string, unique_ptr<BaseExecutor>> statefulExecutors;
		HTTPResponse response;

		while (true)
		{
			try
			{
				HTTPRequest request(sessionsManager, clientIp);

				response.setDefault();

				stream >> request;

				executorsManager.service(move(request), response, statefulExecutors);

				stream << response;
			}
			catch (const web::WebException&)
			{
				data.erase(clientIp);

				for (auto& i : statefulExecutors)
				{
					i.second->destroy();
				}

				break;
			}
			catch (const exceptions::NotImplementedException&)	// 400
			{
				resources->badRequestError(response);

				stream << response;
			}
			catch (const exceptions::FileDoesNotExistException&)	// 404
			{
				resources->notFoundError(response);

				stream << response;
			}
			catch (const out_of_range&)	// 404
			{
				resources->notFoundError(response);

				stream << response;
			}
			catch (const exceptions::BaseExecutorException&)	//500
			{
				resources->internalServerError(response);

				stream << response;
			}
			catch (...)	//500
			{
				resources->internalServerError(response);

				stream << response;
			}
		}
	}

	WebServer::WebServer(const utility::XMLSettingsParser& parser, const filesystem::path& assets, bool isCaching, const string_view& port, DWORD timeout, const string& pathToExecutable) :
		BaseTCPServer(port, timeout, false)
	{
		unordered_map<string, unique_ptr<BaseExecutor>> routes;
		unordered_map<string, createBaseExecutorSubclassFunction> creator;
		unordered_map<string, utility::XMLSettingsParser::ExecutorSettings> settings = parser.getSettings();
		HMODULE executable = LoadLibraryA(pathToExecutable == ini::defaultLoadSourceValue ? nullptr : pathToExecutable.data());

		if (pathToExecutable != ini::defaultLoadSourceValue && !executable)
		{
			throw exceptions::FileDoesNotExistException();
		}

		routes.reserve(settings.size());
		creator.reserve(settings.size());

		for (const auto& [i, j] : settings)
		{
			createBaseExecutorSubclassFunction function = reinterpret_cast<createBaseExecutorSubclassFunction>(GetProcAddress(executable, ("create" + j.name + "Instance").data()));

			if (!function)
			{
				function = reinterpret_cast<createBaseExecutorSubclassFunction>(GetProcAddress(nullptr, ("create" + j.name + "Instance").data()));

				if (!function)
				{
					throw exceptions::CantFindFunctionException();
				}
			}

			switch (j.executorLoadType)
			{
			case utility::XMLSettingsParser::ExecutorSettings::loadType::initialization:
				routes[i] = unique_ptr<BaseExecutor>(function());
				routes[i]->init(j);

				break;

			case utility::XMLSettingsParser::ExecutorSettings::loadType::dynamic:

				break;

			case utility::XMLSettingsParser::ExecutorSettings::loadType::none:
				throw exceptions::MissingLoadTypeException();

				break;
			}

			creator[j.name] = function;
		}

		executorsManager.init(assets, isCaching, move(routes), move(creator), move(settings));
	}
}

#pragma warning(pop)
