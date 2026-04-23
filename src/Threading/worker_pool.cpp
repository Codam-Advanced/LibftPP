#include "Threading/worker_pool.hpp"
#include <cstddef>
#include <mutex>
#include <optional>
#include <stdexcept>

WorkerPool::WorkerPool(size_t amount_of_threads)
{
	_running.store(true);
	for (size_t i = 0; i < amount_of_threads; i++ )
	{
		_workers.emplace_back("Worker " + std::to_string(i), std::bind(&WorkerPool::workerLoop, this));
	}
	for (auto& worker : _workers)
	{
		worker.start();
	}

}

WorkerPool::~WorkerPool()
{
	_running.store(false);
	_notifier.notify_all();

	// terminate al workers
	for (auto& worker : _workers)
    {
        worker.stop();
    }
}

void WorkerPool::addJob(const IJobs& job)
{
	_queue.push_back(job);
	_notifier.notify_one();
}

void WorkerPool::workerLoop()
{
	while (_running.load())
	{
		std::unique_lock<std::mutex> lock(mutex);
		
		_notifier.wait(lock, [this]() {
			return !_running.load() || !_queue.empty();
		});

		if (!_running.load() && _queue.empty())
			break;

		IJobs job = _queue.pop_back();
		lock.unlock(); 

		job();
	}
}