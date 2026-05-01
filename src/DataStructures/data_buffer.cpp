#include "DataStructures/data_buffer.hpp"
#include <cstddef>
#include <cstring>
#include <stdexcept>

DataBuffer::DataBuffer(const char* rawData, size_t size)
{
    _byte_buffer.insert(_byte_buffer.end(), rawData, rawData + size);
}

void DataBuffer::writeToBuffer(const std::string& data)
{
    // to know how how much we need to copy we will also need to add the size of the object serialized data into the string
    uint64_t size = data.size();

    // store the size of the object
    _byte_buffer.insert(_byte_buffer.end(), reinterpret_cast<char*>(&size), reinterpret_cast<char*>(&size) + sizeof(uint64_t));

    // store it at the end of the buffer
    _byte_buffer.insert(_byte_buffer.end(), data.begin(), data.end());
}

std::string DataBuffer::readFromBuffer() const
{
    if (_read_position + sizeof(uint64_t) > _byte_buffer.size())
        throw std::runtime_error("Error: not enough data to read size!");
    
        // the size of the serialized data
    uint64_t size = 0;

    // copy the size and convert it to a size_t
    std::memcpy(&size, _byte_buffer.data() + _read_position, sizeof(uint64_t));

    // offset the read position to pass the size and to the actual data.
    _read_position += sizeof(uint64_t);

    if (_read_position + size > _byte_buffer.size())
        throw std::runtime_error("Error: not enough data to read object!");
    
    // fetch the data from the buffer
    std::string data(_byte_buffer.begin() + _read_position, _byte_buffer.begin() + _read_position + size);

    // update the read position
    _read_position += size;

    // return the data
    return data;
}

const char* DataBuffer::data() const
{
    return _byte_buffer.data();
}

size_t DataBuffer::size() const
{
    return _byte_buffer.size();
}

