#include "utilities.h"

#include <fstream>

#include "HTTPSNetwork.h"

#include "settings.h"

namespace utility
{
	streams::IOSocketStream createSocketStream(DWORD timeout)
	{
		return useHTTPS ?
			streams::IOSocketStream::createStream<web::HTTPSNetwork>("127.0.0.1", "8080", timeout) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>("127.0.0.1", "8080", timeout);
	}

	bool compareFiles(const std::filesystem::path& first, const std::filesystem::path& second)
	{
		constexpr size_t chunkSize = 1ull * 1024 * 1024;

		if (!std::filesystem::exists(first) || !std::filesystem::exists(second))
		{
			std::cout << "Exists error: " << std::filesystem::exists(first) << ' ' << std::filesystem::exists(second) << std::endl;

			return false;
		}

		if (std::filesystem::file_size(first) != std::filesystem::file_size(second))
		{
			std::cout << "Different file sizes: " << std::filesystem::file_size(first) << ' ' << std::filesystem::file_size(second) << std::endl;

			return false;
		}

		std::ifstream firstIn(first, std::ios::binary);
		std::ifstream secondIn(second, std::ios::binary);
		size_t index = 0;
		
		while (!firstIn.eof())
		{
			if (firstIn.get() != secondIn.get())
			{
				std::cout << "Wrong file content at " << index << std::endl;

				return false;
			}

			index++;
		}

		return true;
	}

	std::string_view removeUploadedPrefix(std::string_view data)
	{
		constexpr std::string_view prefix = "uploaded_";

		return data.substr(prefix.size());
	}
}
