#include "TextGenerator.h"

TextGenerator::TextGenerator(std::string_view data) :
	data(data),
	offset(0)
{

}

std::string_view TextGenerator::generate(size_t& size)
{
	constexpr size_t smallStringSize = 15;

	if (offset >= data.size())
	{
		size = 0;

		return {};
	}

	lastResult = data.substr(offset, (std::min)(smallStringSize, data.size() - offset));

	offset += lastResult.size();

	size = lastResult.size();

	return lastResult;
}
