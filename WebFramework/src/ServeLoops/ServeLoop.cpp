#include "ServeLoops/ServeLoop.h"

namespace framework::serve_loop
{
	bool ServeLoop::runLoop(std::unique_ptr<ServeLoop>& loop, std::queue<std::unique_ptr<event::ServeEvent>>& events)
	{
		while (events.size())
		{
			std::unique_ptr<event::ServeEvent> event = std::move(events.front());
			
			(*event)(loop->getStream(), loop);

			events.pop();
		}

		return loop->run();
	}

	ServeLoop::ServeLoop(streams::IOSocketStream&& stream, ResourceExecutor& resources) :
		stream(std::move(stream)),
		resources(resources)
	{

	}

	streams::IOSocketStream& ServeLoop::getStream()
	{
		return stream;
	}

	ResourceExecutor& ServeLoop::getResources()
	{
		return resources;
	}

	const streams::IOSocketStream& ServeLoop::getStream() const
	{
		return stream;
	}
}
