#include "PyChunkGenerator.h"

#include <format>
#include <fstream>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace framework::utility
{
	std::variant<std::string, std::vector<uint8_t>> PyChunkGenerator::generate()
	{
		using ReturnType = std::variant<std::string, std::vector<uint8_t>>;

		PYBIND11_OVERRIDE_PURE
		(
			ReturnType,
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
		std::variant<std::string, std::vector<uint8_t>> chunk = generator.generate();

		if (std::holds_alternative<std::string>(chunk))
		{
			data = std::get<std::string>(chunk);
		}
		else if (std::holds_alternative<std::vector<uint8_t>>(chunk))
		{
			std::span<uint8_t> temp = std::get<std::vector<uint8_t>>(chunk);

			data = std::string(reinterpret_cast<const char*>(temp.data()), temp.size());
		}
		else
		{
			throw std::runtime_error(std::format("Wrong ChunkGenerator return value: {}", chunk.index()));
		}

		size = data.size();

		return data;
	}
}
