#include "Threading/persistent_worker.hpp"
#include "Threading/thread.hpp"
#include <mutex>
#include <stdexcept>
#include <vector>

PersistentWorker::PersistentWorker():
	_running(true),
	_empty(true),
	_worker("PersistentWorker", std::bind(&PersistentWorker::execute, this))

{
	_worker.start();
}

PersistentWorker::PersistentWorker(const std::string& name):
	_running(true),
	_empty(true),
	_worker(name, std::bind(&PersistentWorker::execute, this))
{
	_worker.start();
}

PersistentWorker::~PersistentWorker()
{
	_running.store(false);

	// join the worker thread blocking until its joined
	_worker.stop();
}

void PersistentWorker::addTask(const std::string& name, const IJob& jobToExecute)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_jobs.emplace(name, std::make_shared<IJob>(jobToExecute));

	if (_empty.load())
	{
		_empty.store(false);
		_notifier.notify_all();
	} 
}

void PersistentWorker::removeTask(const std::string& name)
{
	std::lock_guard<std::mutex> lock(_mutex);
	auto it = _jobs.find(name);

	if (it == _jobs.end())
		throw std::runtime_error("No task registered! name: " + name);

	_jobs.erase(it);

	if (_jobs.size() == 0) 
		_empty.store(true);
}

void PersistentWorker::execute()
{
	std::vector<std::shared_ptr<IJob>> jobsToBeExecuted;

	while(_running.load())
	{
		if (_empty.load()) 
			wait();

		if (!_running.load())
			break;

		{
			std::lock_guard<std::mutex> lock(_mutex);
			for (auto& [_, job]: _jobs)
			{
				jobsToBeExecuted.push_back(job);
			}
		}

		for (auto& job : jobsToBeExecuted)
		{
			IJob* func = job.get();
			(*func)();
		}

		jobsToBeExecuted.clear();
	}
}

void PersistentWorker::wait()
{
	std::unique_lock<std::mutex> lock(_mutex);

	_notifier.wait(lock, [this](){
		return !_running.load() || !_empty.load();
	});
}