#include <iostream>
#include <vector>
#include <stack>  // Added for stack-based release
#include <cmath>  // For rounding up to the nearest power of 2
#include "../include/peterson.h"

using namespace std;

class TournamentLock{
private:
    int num_threads;
    int tree_size;  // Adjusted total number of leaves (rounded up to power of 2)
    vector<PetersonLock> locks;
    vector<int> positions; 
    vector<stack<int>> paths;

    //Function to round up to the nearest power of 2
    //  used for when the tree is not full
    int next_power_of_2(int n) {
        return pow(2, ceil(log2(n)));
    }
public:
    //Constructor
    TournamentLock(int n){
        num_threads = n;
        tree_size = next_power_of_2(n);  // Round up to the nearest power of 2
        int num_locks = tree_size - 1;   // Full binary tree internal nodes

        locks.resize(num_locks);
        positions.resize(n);
        paths.resize(n);

        // Assign positions for each thread
        for (int i = 0; i < n; i++) {
            positions[i] = (i + num_locks - 1) / 2; 
        }
    }

    // Acquire lock by moving up the tournament tree
    void acquire(int threadID) {
        int node = positions[threadID];  // Start at assigned lock

        while (node >= 0) {
            int local_thread = threadID % 2;  // Determine thread's role (0 or 1)
            locks[node].acquire(local_thread);
            paths[threadID].push(node);  // Store path for release
            node = (node - 1) / 2;  // Move up the tree
        }
    }

    // Release locks in reverse order (using stack)
    void release(int threadID) {
        while (!paths[threadID].empty()) {
            int node = paths[threadID].top();  // Get top node
            paths[threadID].pop();  // Remove from stack
            int local_thread = threadID % 2;
            locks[node].release(local_thread);
        }
    }
};