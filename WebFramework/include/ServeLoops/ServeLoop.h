#pragma once

#include <memory>
#include <queue>
#include <vector>

#include <IOSocketStream.h>

#include "Events/ServeEvents/ServeEvent.h"
#include "Executors/ResourceExecutor.h"

namespace framework::serve_loop
{
	class ServeLoop
	{
	private:
		std::queue<std::unique_ptr<event::ServeEvent>> events;

	protected:
		streams::IOSocketStream stream;
		ResourceExecutor& resources;

	protected:
		virtual bool serve() = 0;

	public:
		ServeLoop(streams::IOSocketStream&& stream, ResourceExecutor& resources);

		/**
		 * @brief Loop some serve actions
		 * @return true if finished
		 */
		bool run();

		virtual ~ServeLoop() = default;
	};
}
