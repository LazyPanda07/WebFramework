#include "ExecutorsManager.h"

using namespace std;

namespace framework
{
	void ExecutorsManager::service(web::HTTPParser&& request, string& response)
	{
		try
		{
			const string& method = request.getMethod();

			if (method == "GET")
			{

			}
			else if (method == "POST")
			{

			}

		}
		catch (const exceptions::ExecutorException&)
		{

		}
		catch (const out_of_range&)
		{

		}
	}
}
