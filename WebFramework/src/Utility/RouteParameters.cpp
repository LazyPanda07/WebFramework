#include "RouteParameters.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		RouteParameters::RouteParameters(const string& xmlRoute)
		{
			size_t startParameters = xmlRoute.find("/{");
			string parameter;

			baseRoute = string(xmlRoute.begin(), xmlRoute.begin() + startParameters);

			for (size_t i = startParameters + 2; i < xmlRoute.size(); i++)
			{
				if (xmlRoute[i] == '}')
				{
					if (parameter.find(stringType) != string::npos)
					{
						parameters.insert(make_pair(parameter.substr(stringType.size()), string()));
					}
					else if (parameter.find(integerType) != string::npos)
					{
						parameters.insert(make_pair(parameter.substr(integerType.size()), int()));
					}
					else
					{
						parameters.insert(make_pair(parameter, string()));
					}

					parameter.clear();

					i += 2;	//skip /{ symbols in next parameter

					continue;
				}
				
				parameter += xmlRoute[i];
			}
		}

		RouteParameters::RouteParameters(RouteParameters&& other) noexcept :
			baseRoute(move(other.baseRoute)),
			parameters(move(other.parameters))
		{

		}
	}
}
