#include "PyChunkGenerator.h"

#include <pybind11/pybind11.h>

namespace framework::utility
{
	ChunkGeneratorReturnType PyChunkGenerator::generate()
	{
		PYBIND11_OVERRIDE_PURE
		(
			ChunkGeneratorReturnType,
			IPyChunkGenerator,
			generate
		);
	}

	ChunkGeneratorWrapper::ChunkGeneratorWrapper(IPyChunkGenerator& generator) :
		generator(generator)
	{

	}

	std::string_view ChunkGeneratorWrapper::generate(size_t& size)
	{
		const auto& [key, value] = generator.generate();

		size = key.size() == value ? value : key.size();

		return key;
	}
}
