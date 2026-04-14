#include "Utility/ReadOnlyBuffer.h"

#include <algorithm>

namespace framework::utility
{
	std::streamsize ReadOnlyBuffer::xsgetn(char* data, std::streamsize dataSize)
	{
		std::streamsize availableBytes = size - currentSize;
		std::streamsize readBytes = std::clamp<std::streamsize>(dataSize, 0, availableBytes);

		if (!readBytes)
		{
			return readBytes;
		}

		std::memcpy(data, this->data.data() + currentSize + offset, readBytes);

		currentSize += readBytes;

		return readBytes;
	}

	std::streambuf::pos_type ReadOnlyBuffer::seekoff(std::streambuf::off_type off, std::ios_base::seekdir way, std::ios_base::openmode which)
	{
		switch (way)
		{
		case std::ios_base::beg:
			currentSize = off;

			break;

		case std::ios_base::cur:
			currentSize += off;

			break;

		case std::ios_base::end:
			currentSize = size - off;

			break;
		}

		return offset + currentSize;
	}

	ReadOnlyBuffer::ReadOnlyBuffer(std::string_view data, off_type offset, std::streamsize size) :
		data(data),
		offset(offset),
		size(size),
		currentSize(0)
	{

	}
}
