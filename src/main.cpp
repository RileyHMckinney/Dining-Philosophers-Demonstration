#include <iostream>
#include <thread>
#include "peterson.h"

using namespace std;

PetersonLock petersonLock;  // Create a shared lock

void thread_func(int threadID) {
    petersonLock.acquire(threadID);
    cout << "Thread " << threadID << " is in the critical section\n";
    petersonLock.release(threadID);
}

int main() {
    std::thread t1(thread_func, 0);
    std::thread t2(thread_func, 1);

    t1.join();
    t2.join();

    return 0;
}
