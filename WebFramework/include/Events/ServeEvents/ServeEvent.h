#pragma once

#include <IOSocketStream.h>
#include <Http/HttpsNetwork.h>

namespace framework::event
{
	class ServeEvent
	{
	public:
		virtual void operator ()(streams::IOSocketStream& stream) const = 0;

		virtual ~ServeEvent() = default;
	};
}
