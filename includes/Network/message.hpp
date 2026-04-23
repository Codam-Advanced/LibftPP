#pragma once

#include "DataStructures/data_buffer.hpp"
class Message 
{

	public:
		using Type = int;
		
		Message(Type type);

		template<typename TType>
		Message& operator<<(const TType& data)
		{
			_body << data;
		}

		template<typename TType>
		const Message& operator>>(TType& data) const
		{
			_body >> data;
		}

		Type type() const;

		const char* data() const;
		size_t dataSize() const;
	private:
		Type _type;
		DataBuffer _body;

};