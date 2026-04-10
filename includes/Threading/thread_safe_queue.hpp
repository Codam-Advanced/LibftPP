#pragma once

#include <atomic>
#include <deque>
template<typename TType> class ThreadSafeQueue
{

	public:
		void push_back(const TType& newElement)
		{

		}
		void push_front(const TType& newElement)
		{
			
		}
		TType pop_front()
		{
			queue.
		}
		TType pop_back()
		{
			
		}

	private:
		std::atomic<std::deque<TType>> queue;

};