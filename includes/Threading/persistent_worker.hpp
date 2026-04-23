#pragma once

#include "Threading/thread.hpp"
#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <string>
class PersistentWorker
{
	using IJob = std::function<void()>;


	public:
		PersistentWorker();
		PersistentWorker(const std::string& name);
		~PersistentWorker();

		void addTask(const std::string& name, const IJob& jobToExecute);

		void removeTask(const std::string& name);

	private:
		void execute();

		void wait();

		std::mutex _mutex;
		std::map<std::string, std::shared_ptr<IJob>> _jobs;
		
		// is their an active thread?
		std::atomic<bool> _running;

		// is there something to execute
		std::atomic<bool> _empty;

		// notifier towards the thread
		std::condition_variable _notifier;

		// the worker
		Thread _worker;


};