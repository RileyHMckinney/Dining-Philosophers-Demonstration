#include "fine_grained.h"

//Constructor
FineGrainedLock::FineGrainedLock(int n) : locks(n), numLocks(n) {}

//a thread attempts to aquire its neighboring locks
//      - even threadIDs reach left first, and then right 
//      - odd threadIDs reach right first and then left
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

//a thread releases its locks, left first and then right. 
//      Order of release does not change the func of the program.
void FineGrainedLock::release(int threadID) {
    int left = threadID;
    int right = (threadID == 0) ? (numLocks - 1) : (threadID - 1);

    locks[left].release(0);
    locks[right].release(1);
}
