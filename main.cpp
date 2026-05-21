#include <chrono>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "Bonus/timeout.hpp"
#include "DesignPatterns/singleton.hpp"
#include "libftpp.hpp"

// ============================================================
// COLORS
// ============================================================

namespace Color
{
    constexpr const char* Reset   = "\033[0m";

    constexpr const char* Red     = "\033[31m";
    constexpr const char* Green   = "\033[32m";
    constexpr const char* Yellow  = "\033[33m";
    constexpr const char* Blue    = "\033[34m";
    constexpr const char* Magenta = "\033[35m";
    constexpr const char* Cyan    = "\033[36m";
    constexpr const char* White   = "\033[37m";

    constexpr const char* Bold    = "\033[1m";
}

// ============================================================
// UTILITIES
// ============================================================

void clearScreen()
{
    system("clear");
}

void waitForNext()
{
    std::cout << "\n";
    std::cout << Color::Yellow
              << "[ENTER]"
              << Color::Reset
              << " Next test...";
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printHeader(const std::string& title)
{
    clearScreen();

    std::cout << Color::Cyan
              << Color::Bold
              << "====================================================\n"
              << "  " << title << "\n"
              << "====================================================\n"
              << Color::Reset
              << "\n";
}

void printSuccess(const std::string& text)
{
    std::cout << Color::Green << "[SUCCESS] " << Color::Reset
              << text << "\n";
}

void printInfo(const std::string& text)
{
    std::cout << Color::Blue << "[INFO]    " << Color::Reset
              << text << "\n";
}

void printWarning(const std::string& text)
{
    std::cout << Color::Yellow << "[WARNING] " << Color::Reset
              << text << "\n";
}

void printError(const std::string& text)
{
    std::cout << Color::Red << "[ERROR]   " << Color::Reset
              << text << "\n";
}

// ============================================================
// TESTS
// ============================================================

void runPoolTest()
{
    printHeader("POOL TEST");

    Pool<TestObject> pool;

    printInfo("Preallocating 5 objects...");
    pool.resize(5);

    auto obj1 = pool.acquire(15);
    obj1->sayHello();

    {
        auto obj2 = pool.acquire();
        obj2->sayHello();
    }

    auto obj3 = pool.acquire();
    obj3->sayHello();

    printSuccess("Pool test completed.");
}

void runDataBufferTest()
{
    printHeader("DATA BUFFER TEST");

    DataBuffer buffer;

    buffer << std::string("HELLO WORLD") << 42;

    std::string text;
    int value;

    buffer >> text;
    buffer >> value;

    printInfo("Extracted values:");

    std::cout << "String : " << text  << "\n";
    std::cout << "Integer: " << value << "\n";

    printSuccess("DataBuffer test completed.");
}

void runSingletonTest()
{
    printHeader("SINGLETON TEST");

    Singleton<TestCar>::instantiate("blue", "FAST VROOM");

    TestCar* car = Singleton<TestCar>::instance();

    car->vroom();

    try
    {
        Singleton<TestCar>::instantiate("green", "slow vroom");
    }
    catch (const std::exception& e)
    {
        printError(e.what());
    }

    printSuccess("Singleton test completed.");
}

void runObserverTest()
{
    printHeader("OBSERVER TEST");

    enum class EventType
    {
        EventOne,
        EventTwo
    };

    Observer<EventType> observer;

    observer.subscribe(EventType::EventOne, []()
    {
        printInfo("Event One triggered.");
    });

    observer.subscribe(EventType::EventTwo, []()
    {
        printInfo("Event Two - Subscriber A");
    });

    observer.subscribe(EventType::EventTwo, []()
    {
        printInfo("Event Two - Subscriber B");
    });

    printWarning("Notify EventOne");
    observer.notify(EventType::EventOne);

    printWarning("Notify EventTwo");
    observer.notify(EventType::EventTwo);

    printSuccess("Observer test completed.");
}

void runMementoTest()
{
    printHeader("MEMENTO TEST");

    SaveableObject object;

    printInfo("Initial state:");
    object.print();

    auto snapshot = object.save();

    object.setName("JACK");
    object.setAge(30);

    printInfo("Modified state:");
    object.print();

    object.load(snapshot);

    printInfo("Restored state:");
    object.print();

    printSuccess("Memento test completed.");
}

void runStateMachineTest()
{
    printHeader("STATE MACHINE TEST");

    enum class State
    {
        Idle,
        Running,
        Paused
    };

    StateMachine<State> sm;

    sm.addState(State::Idle);
    sm.addState(State::Running);
    sm.addState(State::Paused);

    sm.addAction(State::Idle, []()
    {
        printInfo("System is idle.");
    });

    sm.addAction(State::Running, []()
    {
        printInfo("System is running.");
    });

    sm.addAction(State::Paused, []()
    {
        printInfo("System is paused.");
    });

    sm.addTransition(State::Idle, State::Running, []()
    {
        printWarning("Idle -> Running");
    });

    sm.addTransition(State::Running, State::Paused, []()
    {
        printWarning("Running -> Paused");
    });

    sm.update();

    sm.transitionTo(State::Running);
    sm.update();

    sm.transitionTo(State::Paused);
    sm.update();

    printSuccess("StateMachine test completed.");
}

void runVector2Test()
{
    printHeader("VECTOR2 TEST");

    IVector2<int> a(3, 4);
    IVector2<int> b(1, 2);

    auto add = a + b;
    auto sub = a - b;

    std::cout << "A = (" << a.x << ", " << a.y << ")\n";
    std::cout << "B = (" << b.x << ", " << b.y << ")\n\n";

    std::cout << "A + B = (" << add.x << ", " << add.y << ")\n";
    std::cout << "A - B = (" << sub.x << ", " << sub.y << ")\n";

    std::cout << "Dot Product = " << a.dot(b) << "\n";
    std::cout << "Length(A)   = " << a.length() << "\n";

    printSuccess("Vector2 test completed.");
}

void runVector3Test()
{
    printHeader("VECTOR3 TEST");

    IVector3<int> a(3, 4, 1);
    IVector3<int> b(1, 2, 3);

    auto cross = a.cross(b);

    std::cout << "Cross Product:\n";

    std::cout << "("
              << cross.x << ", "
              << cross.y << ", "
              << cross.z << ")\n";

    printSuccess("Vector3 test completed.");
}

void runRandomGeneratorTest()
{
    printHeader("RANDOM GENERATOR TEST");

    Random2DCoordinateGenerator rng;

    for (int y = 0; y < 4; ++y)
    {
        for (int x = 0; x < 4; ++x)
        {
            long long value = rng(x, y);

            std::cout
                << std::setw(16)
                << value
                << " ";
        }

        std::cout << "\n";
    }

    printSuccess("Random generator test completed.");
}

void runPerlinNoiseTest()
{
    printHeader("PERLIN NOISE TEST");

    PerlinNoise2D perlin;

    constexpr int width  = 100;
    constexpr int height = 40;

    constexpr float scale = 0.03f;

    const char gradient[] =
    {
        ' ',
        '.',
        ':',
        '-',
        '=',
        '+',
        '*',
        '#',
        '%',
        '@'
    };

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float value = perlin.sample(x * scale, y * scale);

            value = (value + 1.0f) * 0.5f;

            value = std::clamp(value, 0.0f, 1.0f);

            int index = static_cast<int>(value * 9.0f);

            std::cout << gradient[index] << gradient[index];
        }

        std::cout << "\n";
    }

    printSuccess("Perlin noise test completed.");
}

// ============================================================
// MULTITHREADING TESTS
// ============================================================

void threadedFunctionA()
{
    for (int i = 0; i < 5; ++i)
    {
        threadSafeCout
            << "[Function A] Iteration "
            << i
            << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void threadedFunctionB()
{
    for (int i = 0; i < 5; ++i)
    {
        threadSafeCout
            << "[Function B] Iteration "
            << i
            << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void printNumbers(const std::string& prefix)
{
    threadSafeCout.setPrefix(prefix);

    for (int i = 1; i <= 5; ++i)
    {
        threadSafeCout
            << "Number: "
            << i
            << std::endl;

        std::this_thread::sleep_for(
            std::chrono::milliseconds(100)
        );
    }
}
void testPush(ThreadSafeQueue<int>& queue, int value)
{
    queue.push_front(value);

    threadSafeCout
        << Color::Green
        << "[PUSH] "
        << Color::Reset
        << "Inserted value: "
        << value
        << std::endl;
}
void testPop(ThreadSafeQueue<int>& queue)
{
    try
    {
        int value = queue.pop_front();

        threadSafeCout
            << Color::Cyan
            << "[POP]  "
            << Color::Reset
            << "Retrieved value: "
            << value
            << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        threadSafeCout
            << Color::Red
            << "[ERROR] "
            << Color::Reset
            << e.what()
            << std::endl;
    }
}

void runThreadWrapperTest()
{
    printHeader("THREAD WRAPPER TEST");

    Thread threadA("Thread-A", threadedFunctionA);
    Thread threadB("Thread-B", threadedFunctionB);

    printInfo("Starting threads...");

    threadA.start();
    threadB.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    printWarning("Stopping threads...");

    threadA.stop();
    threadB.stop();

    printSuccess("ThreadWrapper test completed.");
}

void runWorkerPoolTest()
{
    printHeader("WORKER POOL TEST");

    WorkerPool pool(4);

    printInfo("Dispatching 100 jobs...");

    for (int i = 0; i < 100; ++i)
    {
        pool.addJob([i]()
        {
            threadSafeCout
                << "[Job "
                << i
                << "] Running on thread "
                << std::this_thread::get_id()
                << std::endl;

            std::this_thread::sleep_for(
                std::chrono::milliseconds(20)
            );
        });
    }

    printWarning("Waiting for workers...");

    std::this_thread::sleep_for(std::chrono::seconds(3));

    printSuccess("WorkerPool test completed.");
}

void runPersistentWorkerTest()
{
    printHeader("PERSISTENT WORKER TEST");

    PersistentWorker worker;

    printInfo("Adding tasks...");

    worker.addTask("Heartbeat", []()
    {
        threadSafeCout
            << "[Heartbeat] Alive..."
            << std::endl;

        std::this_thread::sleep_for(
            std::chrono::milliseconds(500)
        );
    });

    worker.addTask("Logger", []()
    {
        threadSafeCout
            << "[Logger] Writing logs..."
            << std::endl;

        std::this_thread::sleep_for(
            std::chrono::milliseconds(700)
        );
    });

    std::this_thread::sleep_for(std::chrono::seconds(3));

    printWarning("Removing Heartbeat task...");

    worker.removeTask("Heartbeat");

    std::this_thread::sleep_for(std::chrono::seconds(3));

    printSuccess("PersistentWorker test completed.");
}

void runThreadSafeIOTest()
{
    printHeader("THREAD SAFE IOSTREAM TEST");

    std::string prefix1 = "[Thread 1] ";
    std::string prefix2 = "[Thread 2] ";

    printInfo("Launching two threads with synchronized console output...");

    std::thread thread1(printNumbers, prefix1);
    std::thread thread2(printNumbers, prefix2);

    thread1.join();
    thread2.join();

    printSuccess("ThreadSafeIO test completed.");
}
void runThreadSafeQueueTest()
{
    printHeader("THREAD SAFE QUEUE TEST");

    ThreadSafeQueue<int> queue;

    printInfo("Launching concurrent push/pop operations...");

    std::thread thread1(testPush, std::ref(queue), 10);
    std::thread thread2(testPush, std::ref(queue), 20);

    std::thread thread3(testPop, std::ref(queue));
    std::thread thread4(testPop, std::ref(queue));
    std::thread thread5(testPop, std::ref(queue));

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();

    printSuccess("ThreadSafeQueue test completed.");
}

void runTimeoutTest()
{
    printHeader("TIMEOUT TEST");

    Singleton<Timeout>::instantiate();

    std::shared_ptr<Timeout::Timer> timer_5_seconds = Singleton<Timeout>::instance()->registerTimer(5, [](){
        std::cout << "5 second timer has expired" << std::endl;
    });

    std::shared_ptr<Timeout::Timer> timer_2_seconds = Singleton<Timeout>::instance()->registerTimer(2, [&](){
        std::cout << "2 second timer has expired" << std::endl;
       
    });

    auto start = std::chrono::steady_clock::now();
    bool done = false;

    while (std::chrono::steady_clock::now() - start < std::chrono::seconds(6))
    {
        Singleton<Timeout>::instance()->check();

        if (!done && std::chrono::steady_clock::now() - start > std::chrono::seconds(4))
        {
            std::cout << "removing the 5 second timer" << std::endl;
            Singleton<Timeout>::instance()->removeTimer(timer_5_seconds);
            done = true;
        }
    }
}
void runObservableValueTest() {
    printHeader("OBSERVABLE VALUE TEST");

    ObservableValue<int> value(10);

    int observer1Calls = 0;
    int observer2Calls = 0;

    int observer1LastValue = 0;
    int observer2LastValue = 0;

    // observer 1
    value.observe("observer_1", [&](const int& data)
    {
        observer1Calls++;
        observer1LastValue = data;

        std::cout << "[observer_1] received value: "
                  << data << std::endl;
    });

    // observer 2
    value.observe("observer_2", [&](const int& data)
    {
        observer2Calls++;
        observer2LastValue = data;

        std::cout << "[observer_2] received value: "
                  << data << std::endl;
    });

    printInfo("storing value 42");
    value.store(42);

    // same value should not trigger observers
    printInfo("storing value 42 again");
    value.store(42);

    // disconnect observer 1
    printInfo("disconnecting observer_1");
    value.disconnect("observer_1");

    // only observer 2 should receive this
    printInfo("storing value 99");
    value.store(99);

    printSuccess("observable value test passed");
}

// ============================================================
// TEST REGISTRY
// ============================================================

struct Test
{
    std::string name;
    std::function<void()> function;
};

int main()
{
    std::vector<Test> tests =
    {  

         // Bonus
        {"Timeout",           runTimeoutTest},
        {"ObservableValue",   runObservableValueTest},
        // object storage
        {"Pool",              runPoolTest},
        {"DataBuffer",        runDataBufferTest},
        
        // design patterns
        {"Singleton",         runSingletonTest},
        {"Observer",          runObserverTest},
        {"Memento",           runMementoTest},
        {"StateMachine",      runStateMachineTest},
       
        // Mathematics
        {"Vector2",           runVector2Test},
        {"Vector3",           runVector3Test},
        {"RandomGenerator",   runRandomGeneratorTest},
        {"PerlinNoise",       runPerlinNoiseTest},

        // Threading
        {"ThreadSafeIO",      runThreadSafeIOTest},
        {"ThreadSafeQueue",   runThreadSafeQueueTest},

        {"ThreadWrapper",     runThreadWrapperTest},
        {"WorkerPool",        runWorkerPoolTest},
        {"PersistentWorker",  runPersistentWorkerTest},
    };

    for (size_t i = 0; i < tests.size(); ++i)
    {
        tests[i].function();

        std::cout << "\n";

        std::cout << Color::Magenta
                  << "Test "
                  << (i + 1)
                  << "/"
                  << tests.size()
                  << ": "
                  << tests[i].name
                  << Color::Reset
                  << "\n";

        waitForNext();
    }

    clearScreen();

    std::cout << Color::Green
              << Color::Bold
              << "ALL TESTS COMPLETED SUCCESSFULLY\n"
              << Color::Reset;


    if (std::chrono::steady_clock::now() - std::chrono::steady_clock::now() > std::chrono::milliseconds(2))
    return 0;
}