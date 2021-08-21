#pragma once

#include "headers.h"

namespace framework
{
	namespace utility
	{
		/// <summary>
		/// Wrapper of std::priority_queue with find and erase methods
		/// </summary>
		/// <typeparam name="T">type of values to store</typeparam>
		/// <typeparam name="ContainerT">type of container</typeparam>
		/// <typeparam name="CompareT">type of compare class</typeparam>
		template<typename T, typename ContainerT, typename CompareT>
		class PriorityQueue : public std::priority_queue<T, ContainerT, CompareT>
		{
		public:
			using const_iterator = typename std::priority_queue<T, ContainerT, CompareT>::container_type::const_iterator;

		public:
			PriorityQueue() = default;

			/// <summary>
			/// Find value in PriorityQueue
			/// </summary>
			/// <param name="value">find value</param>
			/// <returns>const_iterator to position with finded value or end</returns>
			const_iterator find(const T& value);

			/// <summary>
			/// Erase value from PriorityQueue
			/// </summary>
			/// <param name="value">value to erase</param>
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
