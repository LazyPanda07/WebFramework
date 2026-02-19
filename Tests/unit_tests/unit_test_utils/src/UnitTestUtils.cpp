#include "UnitTestUtils.h"

#include <fstream>

#include <JsonParser.h>
#include <JsonBuilder.h>

namespace unit_test_utils
{
	void updateConfigRuntimes(const std::filesystem::path& configPath, const utility::parsers::ConsoleArgumentParser& consoleParser)
	{
		json::JsonParser configParser = std::ifstream(configPath);
		json::JsonBuilder result(CP_UTF8);

		{
			json::JsonObject object;

			configParser.getParsedData(object);

			result = json::JsonBuilder(object, CP_UTF8);

			if (!object.contains<std::vector<json::JsonObject>>("runtimes", true))
			{
				return;
			}
		}

		json::JsonObject& webFramework = result["WebFramework"];

		for (const std::string& runtime : consoleParser.getValues<std::string>("runtimes"))
		{
			for (json::JsonObject& object : webFramework["runtimes"])
			{
				if (object["name"].get<std::string>() == runtime)
				{
					object["enabled"] = true;
				}
			}
		}

		std::ofstream(configPath) << result;
	}
}
