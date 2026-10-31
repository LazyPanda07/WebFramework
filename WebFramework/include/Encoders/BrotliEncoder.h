#pragma once

#include "Encoders/Encoder.h"

#ifdef __WITH_BROTLI__

namespace framework::encoders
{
	class BrotliEncoder : public Encoder
	{
	public:
		BrotliEncoder() = default;

		std::string encode(std::string_view data) override;

		std::string_view getName() const override;

		~BrotliEncoder() = default;
	};
}
#endif
