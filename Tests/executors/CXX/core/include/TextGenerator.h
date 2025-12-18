#pragma once

#include <Utility/ChunkGenerator.hpp>

class TextGenerator : public framework::utility::ChunkGenerator
{
private:
	std::string data;
	std::string lastResult;
	size_t offset;

public:
	TextGenerator(std::string_view data);

	std::string_view generate(size_t& size) override;

	~TextGenerator() = default;
};
