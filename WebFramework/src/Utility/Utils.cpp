#include "Utility/Utils.h"

#include <random>

#ifdef __LINUX__
#include <cstdlib>
#else
#include <processenv.h>
#endif

namespace framework::utility
{
	bool isVariableExist(std::string_view key)
	{
#ifdef __LINUX__
		if (getenv(key.data()))
		{
			return true;
		}

		return false;
#else
		DWORD size = GetEnvironmentVariableA(key.data(), nullptr, 0);

		return static_cast<bool>(size);
#endif
	}

	void setEnvironmentVariable(std::string_view key, std::string_view value)
	{
#ifdef __LINUX__
		setenv(key.data(), value.data(), 1);
#else
		SetEnvironmentVariableA(key.data(), value.data());
#endif
	}

	std::string getEnvironmentVariable(std::string_view key)
	{
		std::string result;

#ifdef __LINUX__
		if (const char* value = getenv(key.data()))
		{
			result = value;
		}
#else
		DWORD size = GetEnvironmentVariableA(key.data(), nullptr, 0);

		result.resize(size);

		GetEnvironmentVariableA(key.data(), result.data(), size);
#endif

		return result;
	}

	std::string generateRandomString(size_t size)
	{
		constexpr std::string_view chars =
			"abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"0123456789";

		static thread_local std::mt19937 rng(std::random_device{}());

		std::uniform_int_distribution<> dist(0, static_cast<int>(chars.size() - 1));
		std::string result;

		result.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			result += chars[dist(rng)];
		}

		return result;
	}
}
