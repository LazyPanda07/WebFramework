#include "RouteParameters.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		RouteParameters::RouteParameters(const string& jsonRoute)
		{
			size_t startParameters = jsonRoute.find("/{");
			string parameter;

			baseRoute = string(jsonRoute.begin(), jsonRoute.begin() + startParameters);

			if (baseRoute.empty())
			{
				baseRoute += '/';
			}

			for (size_t i = startParameters + 2; i < jsonRoute.size(); i++)
			{
				if (jsonRoute[i] == '}')
				{
					if (parameter.find(stringType) != string::npos)
					{
						indices.push_back(parameters.try_emplace(parameter.substr(stringType.size()), string()).first->first);
					}
					else if (parameter.find(integerType) != string::npos)
					{
						indices.push_back(parameters.try_emplace(parameter.substr(integerType.size()), int64_t()).first->first);
					}
					else if (parameter.find(doubleType) != string::npos)
					{
						indices.push_back(parameters.try_emplace(parameter.substr(doubleType.size()), double()).first->first);
					}
					else
					{
						indices.push_back(parameters.try_emplace(move(parameter), string()).first->first);
					}

					parameter.clear();

					i += 2;	//skip /{ symbols in next parameter

					continue;
				}
				
				parameter += jsonRoute[i];
			}
		}
	}
}
