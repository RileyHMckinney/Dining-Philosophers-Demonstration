#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <vector>
#include "peterson.h"

class TournamentTreeLock {
private:
    int numLocks;
    std::vector<PetersonLock> locks;

    // Get parent node index in the binary tree
    int getParent(int node);

    // Helper function to compute the next power of 2
    int nextPowerOf2(int n);

public:
    // Constructor
    TournamentTreeLock(int n);

    // Locking and unlocking functions
    void acquire(int threadID);
    void release(int threadID);
};

#endif // TOURNAMENT_H
