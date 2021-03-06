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
						indices.push_back(parameters.insert(make_pair(parameter.substr(stringType.size()), string())).first->first);
					}
					else if (parameter.find(integerType) != string::npos)
					{
						indices.push_back(parameters.insert(make_pair(parameter.substr(integerType.size()), int64_t())).first->first);
					}
					else
					{
						indices.push_back(parameters.insert(make_pair(move(parameter), string())).first->first);
					}

					parameter.clear();

					i += 2;	//skip /{ symbols in next parameter

					continue;
				}
				
				parameter += jsonRoute[i];
			}
		}

		RouteParameters::RouteParameters(RouteParameters&& other) noexcept :
			baseRoute(move(other.baseRoute)),
			parameters(move(other.parameters)),
			indices(move(other.indices))
		{

		}
	}
}
