#include <exception>
#include <iostream>
#include <ostream>
#include <thread>
#include "DesignPatterns/memento.hpp"
#include "DesignPatterns/state_machine.hpp"
#include "Threading/thread_safe_iostream.hpp"
#include "libftpp.hpp"


void testPool()
{
    // Create a Pool for TestObject
    Pool<TestObject> myPool;

    // Resize the pool to pre-allocate 5 objects
    // Should output the 5 "TestObject constructor"
    myPool.resize(5);

    // Acquire an object from the pool
    Pool<TestObject>::Object obj1 = myPool.acquire(15);
    obj1->sayHello();  // Should output: "Hello from TestObject"

    {
        // Acquire another object in a different scope
        Pool<TestObject>::Object obj2 = myPool.acquire();
        obj2->sayHello();  // Should also output: "Hello from TestObject"
        // obj2 is released back to the pool when it goes out of scope
    }

    // Acquire another object; this should give us the object that obj2 pointed to
    Pool<TestObject>::Object obj3 = myPool.acquire();
    obj3->sayHello();  // Should output: "Hello from TestObject"
}

void testDataBuffer()
{
    std::string string("Welcome World");
    DataBuffer buffer;

    buffer << std::string("HELLo WORLD") << 10;

    int newInt;
    std::string teststring;

    buffer >> teststring >> newInt;
    std::cout << "String: " << teststring << " Int: " << newInt << std::endl;
}

void testSingleTon()
{
    Singleton<TestCar>::instantiate("blue", "faster vroom");
    TestCar* car = Singleton<TestCar>::instance();
    car->vroom();

    try
    {
        Singleton<TestCar>::instantiate("green", "slow vroom");
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void testObserver()
{
    enum class EventType {
        EVENT_ONE,
        EVENT_TWO,
        EVENT_THREE
    };


    Observer<EventType> observer;

    // Subscribe to EVENT_ONE
    observer.subscribe(EventType::EVENT_ONE, []() {
        std::cout << "Event One triggered" << std::endl;
    });

    // Subscribe first lambda to EVENT_TWO
    observer.subscribe(EventType::EVENT_TWO, []() {
        std::cout << "Event Two triggered (First subscriber)" << std::endl;
    });

    // Subscribe second lambda to EVENT_TWO
    observer.subscribe(EventType::EVENT_TWO, []() {
        std::cout << "Event Two triggered (Second subscriber)" << std::endl;
    });

    // Triggering EVENT_ONE
    std::cout << "Notify EVENT_ONE" << std::endl;
    observer.notify(EventType::EVENT_ONE);  // Output: "Event One triggered"

    // Triggering EVENT_TWO
    std::cout << "Notify EVENT_TWO" << std::endl;
    observer.notify(EventType::EVENT_TWO);  
    // Output: 
    // "Event Two triggered (First subscriber)"
    // "Event Two triggered (Second subscriber)"
    // The order may differ

    // Triggering EVENT_THREE (No subscriber)
    std::cout << "Notify EVENT_THREE" << std::endl;
    observer.notify(EventType::EVENT_THREE);  // Output: None, as there are no subscribers


}

void testMemento()
{
    SaveableObject saved;

    saved.print();

    Memento::Snapshot save_version_1 = saved.save();

    saved.setAge(30);
    saved.setName("JACK");

    saved.print();

    saved.load(save_version_1);

    saved.print();
}

void testStateMachine()
{
    enum class State {
        Idle,
        Running,
        Paused,
        Stopped
    };

    StateMachine<State> sm;

    sm.addState(State::Idle);
    sm.addState(State::Running);
    sm.addState(State::Paused);
    sm.addState(State::Stopped);

    sm.addAction(State::Idle, [] { std::cout << "System is idle." << std::endl; });
    sm.addAction(State::Running, [] { std::cout << "System is running." << std::endl; });
    sm.addAction(State::Paused, [] { std::cout << "System is paused." << std::endl; });
    // No addAction for State::Stopped, it will use the default empty lambda

    sm.addTransition(State::Idle, State::Running, [] { std::cout << "Transitioning from Idle to Running." << std::endl; });
    sm.addTransition(State::Running, State::Paused, [] { std::cout << "Transitioning from Running to Paused." << std::endl; });
    sm.addTransition(State::Paused, State::Running, [] { std::cout << "Transitioning from Paused to Running." << std::endl; });
    // No addTransition for State::Stopped

    sm.update();  // Should print: "System is idle."
    sm.transitionTo(State::Running);  // Should print: "Transitioning from Idle to Running."
    sm.update();  // Should print: "System is running."
    sm.transitionTo(State::Paused);  // Should print: "Transitioning from Running to Paused."
    sm.update();  // Should print: "System is paused."

    // Transitioning to and from the new State::Stopped
    try {
        sm.transitionTo(State::Stopped);  // Should not print any transition message, and throw an exception
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;  // Handle state not found
    }
    
    try {
    	sm.transitionTo(State::Stopped);  // Should not print anything, default empty lambda is executed
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;  // Handle state not found
    }
    
    try {
        sm.transitionTo(State::Running);  // Should not print any transition message, and throw an exception
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;  // Handle state not found
    }
} 


void printNumbers(const std::string& p_prefix) {
    threadSafeCout.setPrefix(p_prefix);
    for (int i = 1; i <= 5; ++i) {
        threadSafeCout << "Number: " << i << std::endl;
    }
}

void testThreadSafeIOStream() {
    std::string prefix1 = "[Thread 1] ";
    std::string prefix2 = "[Thread 2] ";

    std::thread thread1(printNumbers, prefix1);
    std::thread thread2(printNumbers, prefix2);

    thread1.join();
    thread2.join();
}
/**
 * @brief Main function
 * 
 * @return int 
 */
int main() {

    testPool();
    // testDataBuffer();
    testSingleTon();
    testObserver();
    
    testMemento();  
    testStateMachine();
    testThreadSafeIOStream();

    


    return 0;
}
