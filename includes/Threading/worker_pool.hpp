#pragma once

#include "Threading/thread.hpp"
#include "Threading/thread_safe_queue.hpp"
#include <condition_variable>
#include <functional>
class WorkerPool
{
	public:
		using IJobs = std::function<void()>;

		WorkerPool(size_t amount_of_threads);
		~WorkerPool();
		
		void addJob(const IJobs& jobToExecute);



	private:

		void workerLoop();
		
		ThreadSafeQueue<IJobs> _queue;
		std::vector<Thread> _workers;
		std::condition_variable _notifier;
		std::atomic<bool> _running;
		std::mutex mutex;

	
	
};

