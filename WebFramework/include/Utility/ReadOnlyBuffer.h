#pragma once

#include <streambuf>

namespace framework::utility
{
	class ReadOnlyBuffer : public std::streambuf
	{
	private:
		using off_type = std::streambuf::off_type;

	private:
		std::string_view data;
		off_type offset;
		std::streamsize size;
		size_t currentSize;

	private:
		std::streamsize xsgetn(char* data, std::streamsize dataSize) override;

		std::streambuf::pos_type seekoff(std::streambuf::off_type off, std::ios_base::seekdir way, std::ios_base::openmode which) override;

	public:
		ReadOnlyBuffer(std::string_view data, off_type offset, std::streamsize size);

		~ReadOnlyBuffer() = default;
	};
}
