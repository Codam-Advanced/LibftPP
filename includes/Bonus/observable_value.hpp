#pragma once

#include <functional>
#include <map>
#include <memory>

template<typename T>
class ObservableValue
{
	public:

		using ObserverEvent = std::function<void(const T& data)>;

		ObservableValue(const T& value)
		{
			// create the unique value
			data = std::make_unique<T>(value);
		}

		ObservableValue(ObservableValue&& other)
		{
			data = std::move(other.data);
			observers = std::move(other.observers);
		}

		// allowing only moving the value
		ObservableValue(ObservableValue& other) = delete;

		void store(const T& value)
		{
			// if we have the same value we can return
			if (value == *data.get())
				return;

			// store the value
			*data = value;

			// call the observer with the new value
			for (auto& [name, observer] : observers)
				observer(*(data.get()));
		}

		void observe(const std::string& name, ObserverEvent callback) { observers.emplace(name, callback); }
		void disconnect(const std::string& name) { observers.erase(name); }

		T* operator->()
		{
			// return the data
			return data.get();
		}

	private:
		
		std::unique_ptr<T> data;
		std::map<std::string, ObserverEvent> observers;
};