#include "fine_grained.h"

FineGrainedLock::FineGrainedLock(int n) : locks(n), numLocks(n) {}

void FineGrainedLock::acquire(int threadID) {
    int left = threadID;
    int right = (threadID == 0) ? (numLocks - 1) : (threadID - 1);

    // If an even-numbered thread, reach left first
    if ((threadID % 2) == 0) {
        locks[left].acquire(0);
        locks[right].acquire(1);
    } else { // If an odd-numbered thread, reach right first
        locks[right].acquire(1);
        locks[left].acquire(0);
    }
}

void FineGrainedLock::release(int threadID) {
    int left = threadID;
    int right = (threadID == 0) ? (numLocks - 1) : (threadID - 1);

    locks[left].release(0);
    locks[right].release(1);
}
