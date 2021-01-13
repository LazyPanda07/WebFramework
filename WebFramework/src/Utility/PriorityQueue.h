#pragma once

#include <queue>

namespace framework
{
	namespace utility
	{
		template<typename T, typename ContainerT, typename CompareT>
		class PriorityQueue : public std::priority_queue<T, ContainerT, CompareT>
		{
		public:
			using const_iterator = typename std::priority_queue<T, ContainerT, CompareT>::container_type::const_iterator;

		public:
			PriorityQueue() = default;

			const_iterator find(const T& value);

			void erase(const T& value);

			~PriorityQueue() = default;
		};

		template<typename T, typename ContainerT, typename CompareT>
		typename PriorityQueue<T, ContainerT, CompareT>::const_iterator PriorityQueue<T, ContainerT, CompareT>::find(const T& value)
		{
			auto first = this->c.cbegin();
			auto last = this->c.cend();

			while (first != last)
			{
				if (*first == value)
				{
					return first;
				}

				++first;
			}

			return last;
		}

		template<typename T, typename ContainerT, typename CompareT>
		void PriorityQueue<T, ContainerT, CompareT>::erase(const T& value)
		{
			this->c.erase(this->find(value));
		}
	}
}
