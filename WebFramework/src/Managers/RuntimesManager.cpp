#include "RuntimesManager.h"

#include <format>

#include "Runtimes/PythonRuntime.h"
#include "Runtimes/DotNetRuntime.h"

namespace framework::runtime
{
	RuntimesManager::Iterator::Iterator(std::unordered_map<size_t, Runtime*>::iterator it) :
		it(it)
	{

	}

	RuntimesManager::Iterator& RuntimesManager::Iterator::operator ++()
	{
		++it;

		return *this;
	}

	bool RuntimesManager::Iterator::operator == (const Iterator& other) const noexcept
	{
		return it == other.it;
	}

	Runtime& RuntimesManager::Iterator::operator *()
	{
		return *it->second;
	}

	const Runtime& RuntimesManager::Iterator::operator *() const
	{
		return *it->second;
	}

	Runtime* RuntimesManager::Iterator::operator ->()
	{
		return it->second;
	}

	const Runtime* RuntimesManager::Iterator::operator ->() const
	{
		return it->second;
	}

	RuntimesManager& RuntimesManager::get()
	{
		static RuntimesManager instance;

		return instance;
	}

	RuntimesManager::Iterator RuntimesManager::begin()
	{
		return Iterator(runtimes.begin());
	}

	RuntimesManager::Iterator RuntimesManager::end()
	{
		return Iterator(runtimes.end());
	}

	Runtime& RuntimesManager::getRuntime(utility::LoadSourceType type)
	{
		switch (type)
		{
		case framework::utility::LoadSourceType::python:
#ifdef __WITH_PYTHON_EXECUTORS__
			return this->getRuntime<PythonRuntime>();
#else
			throw std::runtime_error("Can't get Python runtime. WebFramework built without Python Executor support");
#endif

		case framework::utility::LoadSourceType::dynamicLibrary:
			throw std::runtime_error("No runtime for LoadSourceType::dynamicLibrary");

		case framework::utility::LoadSourceType::dotNet:
#ifdef __WITH_DOT_NET_EXECUTORS__
			return this->getRuntime<DotNetRuntime>();
#else
			throw std::runtime_error("Can't get .NET runtime. WebFramework built without .NET Executor support");
#endif

		default:
			throw std::runtime_error(std::format("No runtime for {}", static_cast<int>(type)));
		}
	}

	const Runtime& RuntimesManager::getRuntime(utility::LoadSourceType type) const
	{
		switch (type)
		{
		case framework::utility::LoadSourceType::python:
#ifdef __WITH_PYTHON_EXECUTORS__
			return this->getRuntime<PythonRuntime>();
#else
			throw std::runtime_error("Can't get Python runtime. WebFramework built without Python Executor support");
#endif

		case framework::utility::LoadSourceType::dynamicLibrary:
			throw std::runtime_error("No runtime for LoadSourceType::dynamicLibrary");

		case framework::utility::LoadSourceType::dotNet:
#ifdef __WITH_DOT_NET_EXECUTORS__
			return this->getRuntime<DotNetRuntime>();
#else
			throw std::runtime_error("Can't get .NET runtime. WebFramework built without .NET Executor support");
#endif

		default:
			throw std::runtime_error(std::format("No runtime for {}", static_cast<int>(type)));
		}
	}
}
