#include "WebFrameworkDynamicPagesFunctions.h"

using namespace std;

namespace framework
{
	string print(const vector<string>& arguments)
	{
		string result;

		for (const auto& i : arguments)
		{
			result += i + ' '; 
		}

		result.pop_back(); 

		return result;
	}
}
