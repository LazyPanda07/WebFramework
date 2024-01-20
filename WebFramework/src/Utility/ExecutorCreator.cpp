#include "ExecutorCreator.h"

namespace framework
{
	namespace utility
	{
		ExecutorCreator::ExecutorCreator() :
			cxxFunction(nullptr),
			cFunction(nullptr)
		{

		}

		void ExecutorCreator::setCreateFunction(createBaseExecutorSubclassFunction createFunction)
		{
			cxxFunction = createFunction;
		}

		void ExecutorCreator::setCreateFunction(void* (*createFunction)())
		{
			cFunction = createFunction;
		}

		ExecutorCreator::operator bool() const
		{
			return cxxFunction || cFunction;
		}
	}
}
