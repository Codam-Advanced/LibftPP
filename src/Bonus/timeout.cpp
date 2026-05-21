#include "Bonus/timeout.hpp"
#include "Threading/thread_safe_iostream.hpp"
#include <ctime>
#include <memory>

std::shared_ptr<Timeout::Timer> Timeout::registerTimer(size_t timeout, std::function<void()> callback)
{
	// create a shared ptr
	std::shared_ptr<Timer> timer = std::make_shared<Timer>();

	// populate the timer with the current time of registration
	// the timeout when surpassed will call the callback
	time(&timer->time_of_registration);
	timer->timeout = timeout;
	timer->callback = callback;

	// store the timer
	timers.push_back(timer);

	// return a reference to the shared ptr so it may be unregistered in a later point of time
	return timer;
}

void Timeout::removeTimer(std::shared_ptr<Timer> timer)
{
	// find the requested timer
	auto it = std::find(timers.begin(), timers.end(), timer);

	// did we find a match
	if (it != timers.end())
	{
		// remove the element from the active timers
		timers.erase(it);
	}
	else threadSafeCout << "NO Timer found!" << std::endl;
}

void Timeout::check()
{
	// iterate through the timers to check if one of them surpassed the timer
	for (auto it = timers.begin(); it != timers.end(); it++)
	{
		time_t current_time;
		time(&current_time);
	
		// if the time has surpassed the current time then we call the callback
		if (current_time - ((*it)->time_of_registration) > static_cast<long>((*it)->timeout))
		{
			// call the callback
			(*it)->callback();

			// remove it since it expired
			it = timers.erase(it);

			if (it ==  timers.end()) return;
		}
	}
}