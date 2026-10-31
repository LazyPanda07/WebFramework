#include "Encoders/GzipEncoder.h"

#ifdef __WITH_ZLIB__

#include <stdexcept>
#include <vector>
#include <iterator>

namespace framework::encoders
{
	GzipEncoder::GzipEncoder() :
		stream({})
	{
		constexpr int32_t memoryLevel = 8;
		constexpr int32_t gzipHeader = 16;

		int errorCode = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + gzipHeader, memoryLevel, Z_DEFAULT_STRATEGY);

		if (errorCode != Z_OK)
		{
			throw std::runtime_error(zError(errorCode));
		}
	}

	std::string GzipEncoder::encode(std::string_view data)
	{
		constexpr size_t bufferSize = 2 * 1024; // 2 KiB

		int errorCode = 0;
		std::vector<uint8_t> buffer(bufferSize);
		std::string result;

		stream.next_in = reinterpret_cast<uint8_t*>(const_cast<char*>(data.data()));
		stream.avail_in = static_cast<uInt>(data.size());

		do
		{
			stream.next_out = buffer.data();
			stream.avail_out = static_cast<uInt>(buffer.size());

			errorCode = deflate(&stream, Z_FINISH);

			if (errorCode != Z_OK && errorCode != Z_STREAM_END)
			{
				throw std::runtime_error(zError(errorCode));
			}

			size_t produced = buffer.size() - stream.avail_out;

			std::copy(buffer.begin(), buffer.begin() + produced, std::back_inserter(result));
		}
		while (errorCode != Z_STREAM_END);

		return result;
	}

	std::string_view GzipEncoder::getName() const
	{
		constexpr std::string_view name = "gzip";

		return name;
	}

	GzipEncoder::~GzipEncoder()
	{
		deflateEnd(&stream);
	}
}
#endif
