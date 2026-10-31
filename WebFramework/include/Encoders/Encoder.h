#pragma once

#include <string>
#include <memory>

namespace framework::encoders
{
	class Encoder
	{
	public:
		static std::unique_ptr<Encoder> getSupportedEncoder(std::string_view name);

	public:
		Encoder() = default;

		virtual std::string encode(std::string_view data) = 0;

		virtual std::string_view getName() const = 0;

		virtual ~Encoder() = default;
	};
}
