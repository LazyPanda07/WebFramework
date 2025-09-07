#pragma once

#include "StaticFileRenderer.h"

namespace framework
{
	class MDRenderer : public StaticFileRenderer
	{
	public:
		MDRenderer() = default;

		std::string render(std::string_view source) const override;

		std::string_view getExtension() const override;

		~MDRenderer() = default;
	};
}
