#include "StaticFileRenderer.h"

namespace std
{
	size_t hash<framework::StaticFileRenderer>::operator ()(const framework::StaticFileRenderer& renderer) const noexcept
	{
		return std::hash<std::string_view>()(renderer.getExtension());
	}
}
