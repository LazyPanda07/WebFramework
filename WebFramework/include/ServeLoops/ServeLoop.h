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
	public:
		/**
		 * @brief Loop some serve actions
		 * @return true if finished
		 */
		static bool runLoop(std::unique_ptr<ServeLoop>& loop, std::queue<std::unique_ptr<event::ServeEvent>>& events);

	protected:
		streams::IOSocketStream stream;
		ResourceExecutor& resources;

	protected:
		virtual bool run() = 0;

	public:
		ServeLoop(streams::IOSocketStream&& stream, ResourceExecutor& resources);

		streams::IOSocketStream& getStream();

		ResourceExecutor& getResources();

		const streams::IOSocketStream& getStream() const;

		virtual ~ServeLoop() = default;
	};
}
