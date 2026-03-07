#include <iostream>
#include <filesystem>
#include <format>
#include <array>

#include <import.hpp>

int main(int argc, char** argv) try
{
	for (const auto& it : std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		if (it.is_regular_file())
		{
			if (std::filesystem::path file = it.path(); file.extension() == ".txt")
			{
				std::filesystem::remove(file);
			}
		}
	}

	framework::utility::initializeWebFramework();

	framework::utility::Config config("task_executors_config.json");

	if (argc == 2)
	{
		config.overrideConfiguration("port", std::stoll(argv[1]));
	}

	framework::WebFramework server(config);

	server.start
	(
		true, 
		[&config]()
		{ 
			constexpr std::array<std::string_view, 4> endpoints =
			{
				"cxx",
				"cc",
				"python",
				"csharp"
			};

			std::cout << std::format("Server is running at http://{}:{}...", config.get<std::string>("ip"), config.get<int64_t>("port")) << std::endl; 
			
			for (std::string_view endpoint : endpoints)
			{
				std::cout << std::format("Available endpoint: http://{}:{}/{}", config.get<std::string>("ip"), config.get<int64_t>("port"), endpoint) << std::endl;
			}
		}
	);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;

	exit(1);
}
