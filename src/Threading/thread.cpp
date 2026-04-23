#include "Threading/thread.hpp"
#include "Threading/thread_safe_iostream.hpp"
#include <functional>
#include <thread>

Thread::Thread(const std::string& name, std::function<void()> functToExecute): 
_name(name),
_started(false),
_funcToExecute(functToExecute)
{
}

Thread::Thread(Thread&& other)
{
	_childThread = std::move(other._childThread);
	_started = other._started;
	_name = other._name;
	_funcToExecute = other._funcToExecute;

	// make the other object invalid
	other._started = true;
	other._funcToExecute = [](){};
	other._name = "Invalid!";
}


Thread::~Thread()
{
	stop();
}

void Thread::start()
{
	if (!_started)
		_childThread = std::thread(std::bind(&Thread::execute, this));
	_started = true;
}

void Thread::stop()
{
	if (_childThread.joinable())
	{
		_childThread.join();
		_started = false;
	}
}

void Thread::execute()
{
	// set the prefix and call the method
	threadSafeCout.setPrefix("[Thread: " + _name + "] ");
	_funcToExecute();
}
