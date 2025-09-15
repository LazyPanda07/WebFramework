#pragma once

#include <string>

namespace framework::interfaces
{
	class IStaticFileRenderer
	{
	public:
		virtual std::string render(std::string_view source) const = 0;

		virtual std::string_view getExtension() const = 0;

		virtual ~IStaticFileRenderer() = default;
	};
}
