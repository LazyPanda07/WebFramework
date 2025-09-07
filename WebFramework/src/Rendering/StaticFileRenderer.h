#pragma once

#include <string>

namespace framework
{
	class StaticFileRenderer
	{
	public:
		virtual std::string render(std::string_view source) const = 0;

		virtual std::string_view getExtension() const = 0;

		virtual ~StaticFileRenderer() = default;
	};
}
