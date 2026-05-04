#pragma once

#include "ServeEvent.h"

namespace framework::event
{
	class UpgradeHttpConnect : public ServeEvent
	{
	public:
		UpgradeHttpConnect() = default;

		void operator ()(streams::IOSocketStream& stream) const override;

		~UpgradeHttpConnect() = default;
	};
}
