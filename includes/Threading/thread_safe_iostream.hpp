#pragma once

#include <iostream>
#include <iterator>
#include <mutex>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <string>
class ThreadSafeIOStream
{
	public:

		~ThreadSafeIOStream();

		/**
		 * @brief Set a prefix for each message
		 * 
		 * @param prefix 
		 */
		void setPrefix(const std::string& prefix);

		// operator overloads for printing to std out
		ThreadSafeIOStream& operator<<(std::ostream& (*manipulator)(std::ostream&));

		/**
		 * @brief Ask a user a question through the stdout which the result will be written to dest
		 * 
		 * @tparam T 
		 * @param question 
		 * @param dest 
		 */
		template<typename T>
		void prompt(const std::string& question, T& dest)
		{
			_buffer << question;
			safePrint();
			*this >> dest;
		}

		// operator overloads for printing to std out
		template<typename T>
		ThreadSafeIOStream& operator<<(const T& data)
		{
			// convert the data into a string stream
			std::ostringstream temp;
			temp << data;

			// extract it as an std::string and put into buffer
			const std::string str = temp.str();
			_buffer << str;

			// do we have a new-line inside our buffer. if so FLUSH
			if (str.find('\n') != std::string::npos)
				safePrint();
	
			// allow chaining
			return *this;
		}

		/**
		 * @brief Reads the stdin and write to dest
		 * 
		 * @tparam T 
		 * @param dest 
		 * @return ThreadSafeIOStream& 
		 */
		template<typename T>
		ThreadSafeIOStream& operator>>(T& dest)
		{
			safe_input(dest);
			return *this;
		}

	private:

		void safePrint();

		template<typename T>
		void safe_input(T& dest)
		{
			std::lock_guard<std::mutex> lock(_print_mutex);
			std::cin >> dest;
		}

		static std::mutex _print_mutex;
		std::stringstream _buffer;
		std::string _prefix;
};

extern thread_local ThreadSafeIOStream threadSafeCout;
