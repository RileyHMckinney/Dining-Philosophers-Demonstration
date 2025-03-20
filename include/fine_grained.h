#ifndef FINE_GRAINED_H
#define FINE_GRAINED_H

#include <vector>
#include "peterson.h"

class FineGrainedLock {
private:
    std::vector<PetersonLock> locks;
    int numLocks;

public:
    FineGrainedLock(int n);

    void acquire(int threadID);
    void release(int threadID);
};

#endif // FINE_GRAINED_H
