#include "tournament.h"
#include <iostream>
#include <cmath>

// Constructor with n threads, and correct number of locks
//      Calling 'nextPowerOf2' ensures that the tournament works for 
//       thread numbers that aren't a power of 2
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
    //Convert thread index to its zero-indexed node number, starting from the root
    int node = numLocks + index; 

    // Traverse up the tree, acquiring locks until at root
    while (node > 0) {
        //calculate the parent node
        int parent = getParent(node);
        
        //Aquire the parent lock, entering as 0 if left child, or 1 if right child
        locks[parent].acquire((node - 1) % 2);
        
        //set the current node to its parent index after parent lock is aquired
        node = parent;
    }
}

// Release lock sequence
void TournamentTreeLock::release(int threadID) {
    int node = 0; //start at root
    int treeHeight = (int)std::log2(numLocks + 1); //calcuate height of tree

    // Traverse top-down, releasing locks
    while (treeHeight > 0) {
        int leftChild = (node * 2) + 1;
        int rightChild = (node * 2) + 2;

        //convert the threadID to binary, extract relevant digit from binary number
        //  that indicates the direction of traversal
        int direction = (threadID >> (treeHeight - 1)) & 1;

        //release the node in the correct direction (left = 0, right = 1)
        locks[node].release(direction);
        
        //
        node = (direction == 1) ? rightChild : leftChild;

        //decrement remaining tree height
        treeHeight--;
    }
}
