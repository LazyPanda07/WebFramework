#include "Managers/RuntimesManager.h"

#include <format>

#include "Runtimes/PythonRuntime.h"
#include "Runtimes/DotNetRuntime.h"
#include "Runtimes/CXXRuntime.h"
#include "Runtimes/CCRuntime.h"
#include "Utility/Utils.h"

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
			utility::logAndThrowException<logging::message::noPythonExecutors, logging::category::runtime>();
#endif

		case framework::utility::LoadSourceType::dynamicLibrary:
			utility::logAndThrowException<logging::message::noRuntimeForDynamicLibrary, logging::category::runtime>();

		case framework::utility::LoadSourceType::dotNet:
#ifdef __WITH_DOTNET_EXECUTORS__
			return this->getRuntime<DotNetRuntime>();
#else
			utility::logAndThrowException<logging::message::noDotNetExecutors, logging::category::runtime>();
#endif

		default:
			throw std::runtime_error(std::format("No runtime for LoadSourceType {}", static_cast<int>(type)));
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
			utility::logAndThrowException<logging::message::noPythonExecutors, logging::category::runtime>();
#endif

		case framework::utility::LoadSourceType::dynamicLibrary:
			utility::logAndThrowException<logging::message::noRuntimeForDynamicLibrary, logging::category::runtime>();

		case framework::utility::LoadSourceType::dotNet:
#ifdef __WITH_DOTNET_EXECUTORS__
			return this->getRuntime<DotNetRuntime>();
#else
			utility::logAndThrowException<logging::message::noDotNetExecutors, logging::category::runtime>();
#endif

		default:
			throw std::runtime_error(std::format("No runtime for LoadSourceType {}", static_cast<int>(type)));
		}
	}

	Runtime& RuntimesManager::getRuntime(utility::ExecutorApiType type)
	{
		switch (type)
		{
		case framework::utility::ExecutorApiType::cc:
			return this->getRuntime<CCRuntime>();

		case framework::utility::ExecutorApiType::cxx:
			return this->getRuntime<CXXRuntime>();

		case framework::utility::ExecutorApiType::python:
#ifdef __WITH_PYTHON_EXECUTORS__
			return this->getRuntime<PythonRuntime>();
#else
			utility::logAndThrowException<logging::message::noPythonExecutors, logging::category::runtime>();
#endif

		case framework::utility::ExecutorApiType::csharp:
#ifdef __WITH_DOTNET_EXECUTORS__
			return this->getRuntime<DotNetRuntime>();
#else
			utility::logAndThrowException<logging::message::noDotNetExecutors, logging::category::runtime>();
#endif

		default:
			throw std::runtime_error(std::format("No runtime for ExecutorAPIType {}", static_cast<int>(type)));
		}
	}

	const Runtime& RuntimesManager::getRuntime(utility::ExecutorApiType type) const
	{
		switch (type)
		{
		case framework::utility::ExecutorApiType::cc:
			return this->getRuntime<CCRuntime>();

		case framework::utility::ExecutorApiType::cxx:
			return this->getRuntime<CXXRuntime>();

		case framework::utility::ExecutorApiType::python:
#ifdef __WITH_PYTHON_EXECUTORS__
			return this->getRuntime<PythonRuntime>();
#else
			utility::logAndThrowException<logging::message::noPythonExecutors, logging::category::runtime>();
#endif

		case framework::utility::ExecutorApiType::csharp:
#ifdef __WITH_DOTNET_EXECUTORS__
			return this->getRuntime<DotNetRuntime>();
#else
			utility::logAndThrowException<logging::message::noDotNetExecutors, logging::category::runtime>();
#endif

		default:
			throw std::runtime_error(std::format("No runtime for ExecutorAPIType {}", static_cast<int>(type)));
		}
	}
}
