#pragma once

#include <filesystem>

namespace framework
{
	class LoadBalancer
	{
	private:


	public:
		LoadBalancer(const std::filesystem::path& configurationINIFile);

		~LoadBalancer() = default;
	};
}
