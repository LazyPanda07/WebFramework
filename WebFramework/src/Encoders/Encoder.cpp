#include "Encoders/Encoder.h"

#include "Encoders/GzipEncoder.h"
#include "Encoders/BrotliEncoder.h"

namespace framework::encoders
{
	std::unique_ptr<Encoder> Encoder::getSupportedEncoder(std::string_view name)
	{
#ifdef __WITH_ZLIB__
		if (name == "gzip")
		{
			return std::make_unique<GzipEncoder>();
		}
#endif

#ifdef __WITH_BROTLI__
		if (name == "br")
		{
			return std::make_unique<BrotliEncoder>();
		}
#endif

		return nullptr;
	}
}
