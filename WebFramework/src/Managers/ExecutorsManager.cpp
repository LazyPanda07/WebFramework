#include "ExecutorsManager.h"

using namespace std;

namespace framework
{
	void ExecutorsManager::service(web::HTTPParser&& request, string& response)
	{
		try
		{
			const string& method = request.getMethod();
			string parameters = request.getParameters();

			parameters.resize(parameters.rfind('/'));

			if (method == "GET")
			{
				routes.at(parameters)->doGet(move(request), response);
			}
			else if (method == "POST")
			{
				routes.at(parameters)->doPost(move(request), response);
			}
		}
		catch (const exceptions::BaseExecutorException&)
		{
			throw;
		}
		catch (const out_of_range&)
		{
			throw;
		}
	}
}
