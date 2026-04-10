#include "Threading/thread_safe_iostream.hpp"

thread_local ThreadSafeIOStream threadSafeCout;
std::mutex ThreadSafeIOStream::_print_mutex;

ThreadSafeIOStream::~ThreadSafeIOStream()
{
	if (_buffer.str() != "") 
			safePrint();
}

void ThreadSafeIOStream::setPrefix(const std::string& prefix)
{
	_prefix = prefix;
}

void ThreadSafeIOStream::safePrint()
{
	std::lock_guard<std::mutex> lock(_print_mutex);
	std::cout << _prefix << _buffer.str();
	std::cout.flush();

	_buffer.str("");
	_buffer.clear();
}

ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream& (*manipulator)(std::ostream&))
{
	// call the manipulator in the buffer
	manipulator(_buffer);

	// if we have a endl (which flushes and append a new line) or a flush. We flush
	if (manipulator == static_cast<std::ostream& (*)(std::ostream&)>(std::endl) ||
		manipulator == static_cast<std::ostream& (*)(std::ostream&)>(std::flush))
		safePrint();
	
	// allow chaining
	return *this;
}