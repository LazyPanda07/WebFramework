#include "PyChunkGenerator.h"

#include <format>

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
		std::variant<std::string, std::vector<uint8_t>> temp = generator.generate();

		if (std::holds_alternative<std::string>(temp))
		{
			std::string_view data = std::get<std::string>(temp);

			size = data.size();

			return data;
		}
		else if (std::holds_alternative<std::vector<uint8_t>>(temp))
		{
			std::span<uint8_t> data = std::get<std::vector<uint8_t>>(temp);

			size = data.size();

			return std::string_view(reinterpret_cast<const char*>(data.data()), data.size());
		}
		else
		{
			throw std::runtime_error(std::format("Wrong ChunkGenerator return value: {}", temp.index()));
		}

		constexpr std::string_view defaultEmptyValue = "DefaultEmptyValue";

		return defaultEmptyValue;
	}
}
