#pragma once

#include <variant>

#include <Utility/ChunkGenerator.hpp>

#include <pybind11/pybind11.h>

namespace framework::utility
{
	using ChunkGeneratorReturnType = std::variant<std::string, pybind11::bytes>;

	class IPyChunkGenerator
	{
	public:
		IPyChunkGenerator() = default;

		virtual ChunkGeneratorReturnType generate() = 0;

		virtual ~IPyChunkGenerator() = default;
	};

	class PyChunkGenerator : public IPyChunkGenerator
	{
	public:
		PyChunkGenerator() = default;

		ChunkGeneratorReturnType generate() override;

		~PyChunkGenerator() = default;
	};

	class ChunkGeneratorWrapper : public ChunkGenerator
	{
	private:
		IPyChunkGenerator& generator;

	public:
		ChunkGeneratorWrapper(IPyChunkGenerator& generator);

		std::string_view generate(size_t& size) override;

		~ChunkGeneratorWrapper() = default;
	};
}
