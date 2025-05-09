#pragma once

#include "Utility/ChunkGenerator.h"

class TextGenerator : public framework::utility::ChunkGenerator
{
private:
	std::string data;
	std::string lastResult;
	size_t offset;

public:
	TextGenerator(std::string_view data);

	std::string_view generate() override;

	~TextGenerator() = default;
};
