#include "Encoders/BrotliEncoder.h"

#ifdef __WITH_BROTLI__

#include <stdexcept>

#include <brotli/encode.h>

namespace framework::encoders
{
	std::string BrotliEncoder::encode(std::string_view data)
	{
		size_t resultSize = BrotliEncoderMaxCompressedSize(data.size());

		if (!resultSize)
		{
			throw std::runtime_error("Too large for br");
		}

		std::string result(resultSize, '\0');

		BROTLI_BOOL errorCode = BrotliEncoderCompress
		(
			BROTLI_DEFAULT_QUALITY, 
			BROTLI_DEFAULT_WINDOW, 
			BrotliEncoderMode::BROTLI_MODE_GENERIC, 
			data.size(), 
			reinterpret_cast<const uint8_t*>(data.data()), 
			&resultSize, 
			reinterpret_cast<uint8_t*>(result.data())
		);

		if (errorCode != BROTLI_TRUE)
		{
			throw std::runtime_error("Can't encode with br");
		}

		result.resize(resultSize);

		return result;
	}

	std::string_view BrotliEncoder::getName() const
	{
		constexpr std::string_view name = "br";

		return name;
	}
}
#endif
