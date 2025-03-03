#pragma once

#include <memory>
#include <filesystem>

#include "IOSocketStream.h"

namespace utility
{
	streams::IOSocketStream createSocketStream();

	bool compareFiles(const std::filesystem::path& first, const std::filesystem::path& second);
}
