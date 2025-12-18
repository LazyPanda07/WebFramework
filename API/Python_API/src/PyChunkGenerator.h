#pragma once

#include <variant>

#include <Utility/ChunkGenerator.hpp>

#include <pybind11/pybind11.h>

namespace framework::utility
{
	using ChunkGeneratorReturnType = std::variant<std::string, std::vector<uint8_t>>;

	class IPyChunkGenerator
	{
	public:
		IPyChunkGenerator() = default;

		virtual pybind11::object generate() = 0;

		virtual ~IPyChunkGenerator() = default;
	};

	class PyChunkGenerator : public IPyChunkGenerator
	{
	public:
		PyChunkGenerator() = default;

		pybind11::object generate() override;

		~PyChunkGenerator() = default;
	};

	class ChunkGeneratorWrapper : public ChunkGenerator
	{
	private:
		IPyChunkGenerator& generator;
		std::string data;

	public:
		ChunkGeneratorWrapper(IPyChunkGenerator& generator);

		std::string_view generate(size_t& size) override;

		~ChunkGeneratorWrapper() = default;
	};
}
