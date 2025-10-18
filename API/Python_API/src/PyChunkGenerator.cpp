#include "PyChunkGenerator.h"

#include <pybind11/pybind11.h>

namespace framework::utility
{
	std::string_view PyChunkGenerator::generate()
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string_view,
			ChunkGenerator,
			generate
		);
	}
}
