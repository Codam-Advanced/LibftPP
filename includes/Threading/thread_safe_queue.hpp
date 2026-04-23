#pragma once

#include <atomic>
#include <cstddef>
#include <deque>
#include <memory>
#include <mutex>
#include <stdexcept>
template<typename TType> class ThreadSafeQueue
{
	public:

		ThreadSafeQueue() : _head(nullptr), _tail(nullptr)
		{
		}

		/**
		 * @brief Push an element to the back of the queue
		 * this lock the queue for other threads until this function is processed 
		 *
		 * @param newElement 
		 */
		void push_back(const TType& newElement)
		{
			const std::lock_guard<std::mutex> lock(_lock);
			std::unique_ptr<t_node> node = new_node(newElement);

			t_node* newTail = node.get();
			if (!_head)
			{
				_head = std::move(node);
				_tail = newTail;
			}
			else
			{
				node->prev = _tail;
				_tail->next = std::move(node);
				_tail = newTail;
			}		}

		/**
		 * @brief Push an element to the front of the queue
		 * this lock the queue for other threads until this function is processed 
		 *
		 * @param newElement 
		 */
		void push_front(const TType& newElement)
		{
			const std::lock_guard<std::mutex> lock(_lock);
			std::unique_ptr<t_node> node = new_node(newElement);

			t_node* newHead = node.get();
			if (!_head)
			{
				_tail = newHead;
				_head = std::move(node);
			}
			else
			{
				_head->prev = newHead;
				node->next = std::move(_head);

				_head = std::move(node);
			}
		}

		/**
		 * @brief Pop an element from the front of the queue
		 * this lock the queue for other threads until this function is processed
		 * 
		 * @return TType 
		 */
		TType pop_front()
		{
			const std::lock_guard<std::mutex> lock(_lock);

			if (!_head)
				throw std::runtime_error("Empty buffer!");
			
			TType data = _head->data;

			_head = std::move(_head->next);

			// do we still have a head remove the prev
			// if we dont have a head reset tail pointer to null
			if (_head)
				_head->prev = nullptr;
			else
				_tail = nullptr;
			
			return data;
		}

		/**
		 * @brief Pop an element from the back of the queue
		 * this lock the queue for other threads until this function is processed
		 * 
		 * @return TType 
		 */
		TType pop_back()
		{
			const std::lock_guard<std::mutex> lock(_lock);

			if (!_head)
				throw std::runtime_error("Empty buffer!");

			TType data = _head->data;

			// last element?
			if (_head.get() == _tail)
			{
				// reset the unique pointer
				_head.reset();

				// reset the raw pointer
				_tail = nullptr;
			}
			else 
			{
				// get the new tail (previous)
				t_node* newTail = _tail->prev;
				
				// reset through the next pointer the old tail
				newTail->next.reset();

				// set the tail
				_tail = newTail;
			}

			return data;
		}

		size_t size()
		{
			const std::lock_guard<std::mutex> lock(_lock);
			size_t counter = 0;
			for (auto* node = _tail; node != nullptr; node = node->prev)
				counter++;
			return counter;
		}

		size_t empty()
		{
			const std::lock_guard<std::mutex> lock(_lock);

			return _tail == nullptr;
		}

	private:

		typedef struct s_node
		{
			TType data;
			// the next pointer owns the object
			std::unique_ptr<struct s_node> next;

			// the previous doesn't own the object
			struct s_node* prev;

		} t_node;

		/**
		 * @brief Helper method to allocate a new node and connect it to the dequeue
		 * 
		 * @param newElement 
		 * @return std::shared_ptr<t_node> 
		 */
		std::unique_ptr<t_node> new_node(const TType& newElement)
		{
			// create a new shared pointer
			std::unique_ptr<t_node> node = std::make_unique<t_node>();

			// assign the data to the node
			node->data = newElement;
			node->next = nullptr;
			node->prev = nullptr;

			// return the node
			return node;
		}

		std::mutex _lock;

		std::unique_ptr<t_node> _head;
		t_node* _tail;
};