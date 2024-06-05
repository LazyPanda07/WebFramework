#pragma once

#include <memory>

#include "IOSocketStream.h"

namespace utility
{
	streams::IOSocketStream createSocketStream();

	std::unique_ptr<streams::IOSocketStream> createSocketStreamPointer();
}
