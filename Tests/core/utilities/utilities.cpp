#include "utilities.h"

#include <fstream>

#include "HTTPSNetwork.h"

#include "settings.h"

namespace utility
{
	streams::IOSocketStream createSocketStream()
	{
		static constexpr DWORD timeout = 600'000;

		return useHTTPS ?
			streams::IOSocketStream::createStream<web::HTTPSNetwork>("127.0.0.1", "8080", timeout) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>("127.0.0.1", "8080", timeout);
	}

	bool compareFiles(const std::filesystem::path& first, const std::filesystem::path& second)
	{
		constexpr size_t chunkSize = 1ull * 1024 * 1024;

		if (!std::filesystem::exists(first) || !std::filesystem::exists(second))
		{
			return false;
		}

		if (std::filesystem::file_size(first) != std::filesystem::file_size(second))
		{
			return false;
		}

		std::ifstream firstIn(first, std::ios::binary);
		std::ifstream secondIn(second, std::ios::binary);
		
		while (!firstIn.eof())
		{
			if (firstIn.get() != secondIn.get())
			{
				return false;
			}
		}

		return true;
	}
}
