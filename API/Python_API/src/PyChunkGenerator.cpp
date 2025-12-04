#define PYBIND11_DETAILED_ERROR_MESSAGES

#include "PyChunkGenerator.h"

#include <format>

#include <pybind11/pybind11.h>

template<typename... Ts>
struct VisitHelper : Ts...
{
	using Ts::operator()...;
};

template<typename... Ts>
VisitHelper(Ts...) -> VisitHelper<Ts...>;

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
		ChunkGeneratorReturnType temp = generator.generate();

		std::visit
		(
			VisitHelper
			(
				[this](const std::string& value)
				{
					data = value;
				},
				[this](const std::vector<uint8_t>& value)
				{
					for (size_t i = 0; i < value.size(); i++)
					{
						data[i] = value[i];
					}
				},
				[](auto&& value) -> size_t
				{
					throw std::runtime_error(std::format("Wrong type in ChunkGeneratorWrapper: {}", typeid(value).name()));

					return 0;
				}
			),
			temp
		);

		size = data.size();

		return data;
	}
}
