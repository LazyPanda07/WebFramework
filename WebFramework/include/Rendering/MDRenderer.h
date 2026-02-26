#pragma once

#include "Rendering/IStaticFileRenderer.h"

namespace framework
{
	class MDRenderer : public interfaces::IStaticFileRenderer
	{
	public:
		MDRenderer() = default;

		std::string render(std::string_view source) const override;

		std::string_view getExtension() const override;

		~MDRenderer() = default;
	};
}
