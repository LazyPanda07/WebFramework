#pragma once

#include "Encoders/Encoder.h"

#ifdef __WITH_ZLIB__

#include <zlib.h>

namespace framework::encoders
{
	class GzipEncoder : public Encoder
	{
	private:
		z_stream stream;

	public:
		GzipEncoder();

		std::string encode(std::string_view data) override;

		~GzipEncoder();
	};
}
#endif
