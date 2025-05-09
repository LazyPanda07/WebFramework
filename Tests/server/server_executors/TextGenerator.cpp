#include "TextGenerator.h"

static constexpr size_t smallStringSize = 15;

TextGenerator::TextGenerator(std::string_view data) :
	data(data),
	offset(0)
{

}

std::string_view TextGenerator::generate()
{
	if (offset >= data.size())
	{
		return {};
	}

	lastResult = data.substr(offset, (std::min)(smallStringSize, data.size() - offset));

	offset += lastResult.size();

	return lastResult;
}
