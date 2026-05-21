#pragma once

#include "DesignPatterns/singleton.hpp"
#include <ctime>
#include <functional>
#include <memory>
#include <vector>

class Timeout
{
	// convert the timer into a singleton
	friend class Singleton<Timeout>;

	protected:
		Timeout() = default;

	
	public:

		// the timer object
		struct Timer
		{
			time_t time_of_registration;
			size_t timeout;
			std::function<void()> callback;
		};

		// methods
		std::shared_ptr<Timer> registerTimer(size_t timeout, std::function<void()> callback);
		void removeTimer(std::shared_ptr<Timer> timer);

		/**
		 *	@brief Check whether one of the timers have surpassed.
		 * 	
		 */
		void check();

	private:
		std::vector<std::shared_ptr<Timer>> timers;

};