#include "Network/message.hpp"
#include "DataStructures/data_buffer.hpp"

Message::Message(int type): _type(type) {}

int Message::type() const { return _type; }
size_t Message::dataSize() const { return _body.size(); }
const char* Message::data() const { return _body.data(); }
