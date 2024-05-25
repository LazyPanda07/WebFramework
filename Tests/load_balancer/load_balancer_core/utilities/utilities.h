#pragma once

#include "IOSocketStream.h"

namespace utility
{
	streams::IOSocketStream createSocketStream(bool useHTTPS);
}
