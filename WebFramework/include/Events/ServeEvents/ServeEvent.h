#pragma once

#include <IOSocketStream.h>
#include <Http/HttpsNetwork.h>

namespace framework::serve_loop
{
	class ServeLoop;
}

namespace framework::event
{
	class ServeEvent
	{
	public:
		virtual void operator ()(streams::IOSocketStream& stream, std::unique_ptr<serve_loop::ServeLoop>& loop) const = 0;

		virtual ~ServeEvent() = default;
	};
}
