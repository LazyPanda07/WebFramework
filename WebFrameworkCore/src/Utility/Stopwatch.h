#pragma once

#include <chrono>

namespace framework::utility
{
	namespace concepts
	{
		template<typename T>
		concept Duration = requires(T value)
		{
			{ std::chrono::duration_cast<std::chrono::milliseconds>(value) };
		};
	}

	class Stopwatch
	{
	private:
		static std::chrono::microseconds getTime();

	private:
		std::chrono::microseconds start;

	public:
		Stopwatch();

		void restart();

		template<concepts::Duration T = std::chrono::microseconds>
		T elapsed() const;

		~Stopwatch() = default;
	};
}

namespace framework::utility
{
	template<concepts::Duration T>
	T Stopwatch::elapsed() const
	{
		return std::chrono::duration_cast<T>(Stopwatch::getTime() - start);
	}
}
