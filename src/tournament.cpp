#include "tournament.h"
#include <iostream>
#include <cmath>

// Constructor with n threads, and correct number of locks
TournamentTreeLock::TournamentTreeLock(int n) 
    : numLocks(nextPowerOf2(n) - 1), locks(numLocks) {}

// Get parent node index in the binary tree
int TournamentTreeLock::getParent(int node) {
    return (node - 1) / 2;
}

// Compute the next power of 2 greater than or equal to n
int TournamentTreeLock::nextPowerOf2(int n) {
    if (n <= 1) return 1;
    int power = 1;
    while (power < n) power *= 2;
    return power;
}

// Acquire lock sequence
void TournamentTreeLock::acquire(int threadID) {
    int index = threadID;  // Zero-based thread index
    int node = numLocks + index; // Convert to leaf position

    if (false) {
        std::cout << "Thread " << threadID << " starting acquire sequence...\n";
    }

    // Traverse up the tree, acquiring locks
    while (node > 0) {
        int parent = getParent(node);
        if (false) {
            std::cout << "Thread " << threadID << " acquiring lock at node " << parent << std::endl;
        }

        locks[parent].acquire((node - 1) % 2); // Left child (0) or right child (1)
        node = parent;
    }

    if (false) {
        std::cout << "Thread " << threadID << " has acquired all necessary locks.\n";
    }
}

// Release lock sequence
void TournamentTreeLock::release(int threadID) {
    int node = 0;
    int treeHeight = (int)std::log2(numLocks + 1);

    // Traverse top-down, releasing locks
    while (treeHeight > 0) {
        int leftChild = (node * 2) + 1;
        int rightChild = (node * 2) + 2;

        int direction = (threadID >> (treeHeight - 1)) & 1;

        locks[node].release(direction);
        node = (direction == 1) ? rightChild : leftChild;
        treeHeight--;
    }
}
