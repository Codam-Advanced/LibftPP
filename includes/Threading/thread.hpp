#pragma once
#include <functional>
#include <string>
#include <thread>

class Thread
{
	Thread() = delete;

	public:
		Thread(const std::string& name, std::function<void()> funcToExecute);
		Thread(Thread&& other);
		~Thread();

		void start();

		void stop();

	private:
		std::thread _childThread;
		std::string _name;
		bool _started;
		std::function<void()> _funcToExecute;

		void execute();


};