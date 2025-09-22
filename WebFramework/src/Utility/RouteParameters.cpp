#include "RouteParameters.h"

namespace framework::utility
{
	RouteParameters::RouteParameters(const std::string& jsonRoute)
	{
		size_t startParameters = jsonRoute.find("/{");
		std::string parameter;

		baseRoute = std::string(jsonRoute.begin(), jsonRoute.begin() + startParameters);

		if (baseRoute.empty())
		{
			baseRoute += '/';
		}

		for (size_t i = startParameters + 2; i < jsonRoute.size(); i++)
		{
			if (jsonRoute[i] == '}')
			{
				if (parameter.find(stringType) != std::string::npos)
				{
					indices.push_back(parameters.try_emplace(parameter.substr(stringType.size()), std::string()).first->first);
				}
				else if (parameter.find(integerType) != std::string::npos)
				{
					indices.push_back(parameters.try_emplace(parameter.substr(integerType.size()), int64_t()).first->first);
				}
				else if (parameter.find(doubleType) != std::string::npos)
				{
					indices.push_back(parameters.try_emplace(parameter.substr(doubleType.size()), double()).first->first);
				}
				else
				{
					indices.push_back(parameters.try_emplace(move(parameter), std::string()).first->first);
				}

				parameter.clear();

				i += 2;	//skip /{ symbols in next parameter

				continue;
			}

			parameter += jsonRoute[i];
		}
	}
}
