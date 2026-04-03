#pragma once
#include <map>
#include <functional>


// Test enum which will be used to test the observer
enum ObserverEvents
{
	RAINING,
	SUNNY,
	STORM,
	CLOUDY,
};


template<typename TEvent> class Observer
{
	using Callback = std::function<void()>;

	public:
		Observer() = default;


		/**
		 * @brief subscribe a lambda function to a event
		 * once the event is triggered it will call the lambda function
		 * 
		 * @param event 
		 * @param lambda 
		 */
		void subscribe(const TEvent& event, const Callback& lambda)
		{
			if (_observer.find(event) == _observer.end())
			{
				std::vector<Callback> vec;
				vec.push_back(lambda);

				_observer.insert(std::pair<TEvent, std::vector<Callback>>(event, std::move(vec)));
			}
			else
			{
				std::vector<Callback>& vec = _observer.at(event);
				vec.push_back(lambda);
			}	
		}

		/**
		 * @brief notify all callbacks for a specific event.
		 * trigger an event which will call every callback that subscribed to this event
		 * 
		 * @param event 
		 */
		void notify(const TEvent& event)
		{
			// if we cannot find any subscription return
			if (_observer.find(event) == _observer.end())
				return;

			std::vector<Callback>& vec = _observer.at(event);
			// iterate through all methods to call the callback
			for (auto& fn : vec)
			{
				fn();
			}
		}

	private:
		std::map<TEvent, std::vector<Callback>> _observer;
};