#include "Stopwatch.h"

namespace framework::utility
{
	std::chrono::microseconds Stopwatch::getTime()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());;
	}

	Stopwatch::Stopwatch()
	{
		this->restart();
	}

	void Stopwatch::restart()
	{
		start = Stopwatch::getTime();
	}
}
