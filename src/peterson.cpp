#include "peterson.h"

//Initialize the lock
PetersonLock::PetersonLock() {
    flag[0] = false;
    flag[1] = false;
    turn = 0;
}

//Enter critical section, this "threadID" can only be 0 or 1!!!
void PetersonLock::acquire(int threadID) {
    int other = 1 - threadID;
    flag[threadID] = true;
    turn = other;

    while (flag[other] && turn == other);
}

// Release the lock (exit critical section)
void PetersonLock::release(int threadID) {
    flag[threadID] = false;
}
