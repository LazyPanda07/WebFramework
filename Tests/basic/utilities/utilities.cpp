#include "utilities.h"

#include "HTTPSNetwork.h"

#include "settings.h"

namespace utility
{
	streams::IOSocketStream createSocketStream()
	{
		return streams::IOSocketStream
		(
			useHTTPS ?
			std::make_unique<web::HTTPSNetwork>("localhost", "8080") :
			std::make_unique<web::HTTPNetwork>("localhost", "8080")
		);
	}
}
