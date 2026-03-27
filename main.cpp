#include <exception>
#include <iostream>
#include "libftpp.hpp"


void testPool()
{
    Pool<std::string> pool;

    pool.resize(1);

    try {
        auto object2 = pool.acquire<std::string>("MIEP");
        std::cout << object2->c_str() << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Error with acquire [MIEP]" << e.what() << std::endl;
    }

    try {

        auto object = pool.acquire<std::string>("hello World");
        std::cout << object->c_str() << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error with acquire [Hello world]" << e.what() << std::endl;
    }

    try {

        auto object = pool.acquire<std::string>("Invalid");
        auto object2 = pool.acquire<std::string>("MIEP");
        std::cout << object->c_str() << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error with acquire [invalid]: " << e.what() << std::endl;
    }
}

void testDataBuffer()
{
    std::string string("Welcome World");

    DataBuffer buffer;

    buffer << std::string("HELLo WORLD") << 10;

    int newIn

    std::string teststringt;;

    int invalid;

    buffer >> teststring >> newInt;

    std::cout << "String: " << teststring << " Int: " << newInt << std::endl;
}

/**
 * @brief Main function
 * 
 * @return int 
 */
int main() {

    testPool();
    testDataBuffer();
  

    


    return 0;
}
