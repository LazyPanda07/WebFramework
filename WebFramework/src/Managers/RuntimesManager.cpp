#include "RuntimesManager.h"

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
}
