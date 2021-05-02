#include <iostream>

#include "WebFramework.h"

#pragma comment (lib, "WebFramework.lib")

using namespace std;

int main(int argc, char** argv)
{
	try
	{
		framework::WebFramework webFramework("web_framework_settings.json");
		string s;

		webFramework.startServer();

		while (cin >> s)
		{

		}
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}
