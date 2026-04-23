#pragma once

#include <cstddef>
#include <vector>
#include <sstream>

class DataBuffer
{

    public:
        DataBuffer(): _read_position(0) {};

        // gives an unserialized object and serialize it
        template<typename TType>
        DataBuffer& operator<<(const TType& obj)
        {
            // create an empty string stream
            std::stringstream ss;

            // serialize and save the object in the data buffer
            ss << obj;

            writeToBuffer(ss.str());

            // allow chaining
            return *this;
        }
        
        // unserialize an object and assign it to the argument
        template<typename TType>
        DataBuffer& operator>>(TType& obj)
        {
            // get the serialized data
            std::stringstream serialized_data(readFromBuffer());

            // deserialize the data
            serialized_data >> obj;

            // allow chaining
            return *this;
        }

        // gives an unserialized object and serialize it
        DataBuffer& operator<<(const std::string& obj)
        {
            // since we already have a string we can just pass it to the buffer
            writeToBuffer(obj);

            // allow chaining
            return *this;
        }
        
        // unserialize an object and assign it to the argument
        DataBuffer& operator>>(std::string& obj)
        {
            // we can simply read from the buffer
            obj = readFromBuffer();

            // allow chaining
            return *this;
        }

        // get the total serialized data of the buffer
        const char* data() const;

        // get the size of the serialized data
        size_t size() const;

    private:

        std::string readFromBuffer();

        void writeToBuffer(const std::string& data);

        // read position
        size_t _read_position;

        // an std vector in byte format
        std::vector<char> _byte_buffer;
};