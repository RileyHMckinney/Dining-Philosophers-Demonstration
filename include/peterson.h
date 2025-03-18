#ifndef PETERSON_H
#define PETERSON_H

#include <atomic>

using namespace std;

class PetersonLock {
private:
    atomic<bool> flag[2];
    atomic<int> turn;

public:
    PetersonLock();  // Constructor

    void acquire(int threadID);  // Acquire the lock
    void release(int threadID);  // Release the lock
};

#endif // PETERSON_H
