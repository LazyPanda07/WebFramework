#include "Encoders/Encoder.h"

#include "Encoders/GzipEncoder.h"

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

		return nullptr;
	}
}
