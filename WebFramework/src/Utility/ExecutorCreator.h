#pragma once

#include "Executors/BaseExecutor.h"

namespace framework
{
	namespace utility
	{
		/**
		 * @brief Wrapper around framework::BaseExecutor*(*)() and void*(*)()
		*/
		class WEB_FRAMEWORK_API ExecutorCreator
		{
		private:
			createBaseExecutorSubclassFunction cxxFunction;
			void*(*cFunction)();

		public:
			ExecutorCreator();

			void setCreateFunction(createBaseExecutorSubclassFunction createFunction);

			void setCreateFunction(void* (*createFunction)());

			BaseExecutor* operator()() const noexcept;

			operator bool() const;

			~ExecutorCreator() = default;
		};

		inline BaseExecutor* ExecutorCreator::operator()() const noexcept
		{
			return cxxFunction ?
				cxxFunction() :
				static_cast<BaseExecutor*>(cFunction());
		}
	}
}
