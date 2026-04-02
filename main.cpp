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

    int newInt;
    std::string teststring;
    int invalid;

    buffer >> teststring >> newInt;
    std::cout << "String: " << teststring << " Int: " << newInt << std::endl;
}

void testSingleTon()
{
    Singleton<TestCar> singleton;
    singleton.instantiate("blue", "faster vroom");

    TestCar* car = singleton.instance();
    car->vroom();

    try
    {
        singleton.instantiate("green", "slow vroom");
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void testObserver()
{
    Observer<ObserverEvents> observer;

    observer.subscribe(ObserverEvents::RAINING, std::function([]() {
        std::cout << "Oh no its raining I gotta fetch my rain coat" << std::endl;
    }));

    observer.subscribe(ObserverEvents::STORM, std::function([]() {
        std::cout << "Oh no its STORM I gotta stay inside" << std::endl;
    }));

    observer.subscribe(ObserverEvents::STORM, std::function([]() {
        std::cout << "ALERT ALERT LIATHS STORM INSIDE SHUT THE DOORS" << std::endl;
    }));

    observer.subscribe(ObserverEvents::SUNNY, std::function([]() {
        std::cout << "YAY SUN!" << std::endl;
    }));


    std::cout << "Its beginning to rain...." << std::endl;
    observer.notify(ObserverEvents::RAINING);
    std::cout << std::endl;

    std::cout << "The rain is turning into a storm...." << std::endl;
    observer.notify(ObserverEvents::STORM);
    std::cout << std::endl;

    std::cout << "The storm is over! and the sun comes out..." << std::endl;
    observer.notify(ObserverEvents::SUNNY);
    std::cout << std::endl;
}

/**
 * @brief Main function
 * 
 * @return int 
 */
int main() {

    testPool();
    testDataBuffer();
    testSingleTon();
    testObserver();
  

    


    return 0;
}
