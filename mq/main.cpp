#include <atomic>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <type_traits>

// we define some non-const data on the stack
int data = 0;
std::atomic<bool> ready(false);

// producer thread operation
void producer() {
    std::cout << "Setting data variable to 42" << std::endl;
    data = 42;
    ready.store(true, std::memory_order_release);
}

// consumer thread operation
void consumer() {
    while (!ready.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }

    std::cout << "Data consumed is: ";
    std::cout << data << std::endl;
}

// So ideally we create two threads and change the shared memory by these two
// threads which in this case will be the stack memory of the main function
int main() {
    std::cout << "Initial Value for data variable: " << data << std::endl;

    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();
    return EXIT_SUCCESS;
}