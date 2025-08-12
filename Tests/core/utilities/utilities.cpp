#include "utilities.h"

#include <fstream>

#include "HTTPSNetwork.h"

#include "settings.h"

namespace utility
{
	streams::IOSocketStream createSocketStream()
	{
		return useHTTPS ?
			streams::IOSocketStream::createStream<web::HTTPSNetwork>("127.0.0.1", "8080", 3'600'000) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>("127.0.0.1", "8080", 3'600'000);
	}

	bool compareFiles(const std::filesystem::path& first, const std::filesystem::path& second)
	{
		constexpr size_t chunkSize = 1ull * 1024 * 1024;

		if (!std::filesystem::exists(first) || !std::filesystem::exists(second))
		{
			std::cerr << "Exists error: " << std::filesystem::exists(first) << ' ' << std::filesystem::exists(second) << std::endl;

			return false;
		}

		if (std::filesystem::file_size(first) != std::filesystem::file_size(second))
		{
			std::cerr << "Different file sizes: " << std::filesystem::file_size(first) << ' ' << std::filesystem::file_size(second) << std::endl;

			return false;
		}

		std::ifstream firstIn(first, std::ios::binary);
		std::ifstream secondIn(second, std::ios::binary);
		std::string firstCompare(chunkSize, '\0');
		std::string secondCompare(chunkSize, '\0');

		while (!firstIn.eof())
		{
			firstIn.read(firstCompare.data(), chunkSize);
			secondIn.read(secondCompare.data(), chunkSize);

			if (firstCompare != secondCompare)
			{
				std::cerr << "Chunks does not same at offset: " << firstIn.tellg() << std::endl;

				return false;
			}
		}
		
		return true;
	}

	std::string_view removeUploadedPrefix(std::string_view data)
	{
		constexpr std::string_view prefix = "uploaded_";

		return data.substr(prefix.size());
	}
}
