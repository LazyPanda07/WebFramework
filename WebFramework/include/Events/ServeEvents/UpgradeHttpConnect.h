#pragma once

#include "ServeEvent.h"

namespace framework::event
{
	class UpgradeHttpConnect : public ServeEvent
	{
	public:
		UpgradeHttpConnect() = default;

		void operator ()(streams::IOSocketStream& stream, std::unique_ptr<serve_loop::ServeLoop>& loop) const override;

		~UpgradeHttpConnect() = default;
	};
}
