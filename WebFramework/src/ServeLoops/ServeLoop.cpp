#include "ServeLoops/ServeLoop.h"

namespace framework::serve_loop
{
	ServeLoop::ServeLoop(streams::IOSocketStream& stream, ResourceExecutor& resources) :
		stream(stream),
		resources(resources)
	{

	}

	bool ServeLoop::run()
	{
		while (events.size())
		{
			std::unique_ptr<event::ServeEvent> event = std::move(events.front());

			(*event)(stream);

			events.pop();
		}

		return this->serve();
	}
}
