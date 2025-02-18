#pragma once

#include "LargeBodyHandler.h"

namespace framework::utility
{
	class BaseLargeBodyHandler : public web::LargeBodyHandler
	{
	private:
		bool handleChunk(std::string_view data, size_t& bodySize) override;

	public:
		BaseLargeBodyHandler(web::Network& network);

		~BaseLargeBodyHandler() = default;
	};
}
