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
		std::string_view result;

		size = std::visit
		(
			VisitHelper
			(
				[&result](const std::string& value) -> size_t
				{
					result = value;

					return value.size();
				},
				[&result](pybind11::bytes value) -> size_t
				{
					result = value;

					return pybind11::len(value);
				},
				[](auto&& value) -> size_t
				{
					throw std::runtime_error(std::format("Wrong type in ChunkGeneratorWrapper: {}", typeid(value).name()));

					return 0;
				}
			),
			temp
		);

		return result;
	}
}
