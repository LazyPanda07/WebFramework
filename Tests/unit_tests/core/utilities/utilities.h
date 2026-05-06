#pragma once

#include <memory>
#include <filesystem>

#include "IOSocketStream.h"

namespace utility
{
	streams::IOSocketStream createSocketStream();

	void createWebSocketStream(streams::IOSocketStream& stream);

	bool compareFiles(const std::filesystem::path& first, const std::filesystem::path& second);

	std::string_view removeUploadedPrefix(std::string_view data);
}
