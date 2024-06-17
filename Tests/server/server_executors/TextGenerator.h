#pragma once

#include "Utility/ChunkGenerator.h"

class TextGenerator : public framework::utility::ChunkGenerator
{
private:
	std::string data;
	size_t offset;

public:
	TextGenerator(std::string_view data);

	std::string generate() override;

	~TextGenerator() = default;
};
