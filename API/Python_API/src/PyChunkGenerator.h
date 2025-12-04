#pragma once

#include <tuple>

#include <Utility/ChunkGenerator.hpp>

namespace framework::utility
{
	using ChunkGeneratorReturnType = std::tuple<std::string, size_t>;

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
