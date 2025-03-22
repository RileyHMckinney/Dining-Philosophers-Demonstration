/*Programming Project 2: The Dining Philosophers Problem 
Name: Riley Mckinney
EID: rhm220001
Date: 03/22/2025
Professor: Neeraj Mittal
Section: CS5348.004
Description: This program simulates two alternative solutions to the Dining Philosophers problem.

The 1st solution: Creates a tournament tree of locks, only one philosopher is allowed to dine at a time
The 2nd solution: Orients the philosophers in a circle, and each chopstick is a lock shared by 
    neighboring philosophers

- main.cpp: driving program, calls on tournament.cpp and fine_grained.cpp 
- tournament.cpp: a class which structures peterson's algorithm locks in a tree structure, 
    enforcing mutual exclusion
- fine_grained.cpp: a class which structures peterson's alogorithm locks in a circular structure,
    where each thread shares a lock with its neighbors
- peterson.cpp: a class which implements the peterson's algorithm for mutual exclusion between two threads
*/


#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include "peterson.h"
#include "tournament.h"
#include "fine_grained.h"

using namespace std;

// Function to simulate variable-length sleeping (thinking/eating)
void random_sleep();

//Coarse-grained synchro approach (the tournament tree system)
void coarse_grained(int threadID, TournamentTreeLock& tournamentLock);

//Fine_grained synchro approach (each "chopstick" is a lock)
void fine_grained(int threadID, FineGrainedLock& fineGrainedLock);

//main driver fnc
int main(int argc, char* argv[]) {
    // Ensure correct usage
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <number_of_philosophers>\n";
        return 1;
    }

    // Read and validate the number of philosophers (threads)
    int numThreads = stoi(argv[1]);
    if (numThreads < 2) {
        cerr << "Error: Number of philosophers must be at least 2.\n";
        return 1;
    }

    //formatted print statement
    cout << "|----------------------------------------------------------------------------|" << endl;
    cout << "| Simulate the Dining Philosophers problem using the coarse-grained approach.|" << endl;
    cout << "|----------------------------------------------------------------------------|" << endl;

    // Create an isntance of the tournament lock with the specified number of threads
    TournamentTreeLock tournamentLock(numThreads);

    // Launch threads
    vector<thread> threads;
    for (int i = 0; i < numThreads; i++) {
        threads.push_back(thread(coarse_grained, i, ref(tournamentLock)));
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    //clear the threads before running next synchonization technique
    threads.clear();

    //formatted print statement
    cout << "|--------------------------------------------------------------------------|" << endl;
    cout << "| Simulate the Dining Philosophers problem using the fine-grained approach.|" << endl;
    cout << "|--------------------------------------------------------------------------|" << endl;

    //Create an instance of the FineGrainedLock class with the specified number of threads
    FineGrainedLock fineGrainedLock(numThreads);

    //launch the threads on the same threads vector
    for (int i = 0; i < numThreads; i++) {
        threads.push_back(thread(fine_grained, i, ref(fineGrainedLock)));
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    cout << "All simulations have been completed.\n";
    return 0;
}

//Randomize the sleep duration using a uniform distribution from 1-500ms
void random_sleep() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 500);
    this_thread::sleep_for(chrono::milliseconds(dist(gen)));
}

//Calls on the tournament lock class function
void coarse_grained(int threadID, TournamentTreeLock& tournamentLock) {
    //philosopher thinks (can be done concurrently with other philosophers)
    //      This output will be garbled and overlap with other threads 
    //      because there is intentionally no mutex in place.
    cout << "Philo " << threadID << " has started thinking." << endl;
    random_sleep();
    cout << "Philo " << threadID << " has finished thinking." << endl;

    //philosopher eats (can only be done one philosopher at a time)
    
    //Philosopher indicates desire to aquire the lock
    //      Locks are aquired from bottom up.
    //      spins until lock is aquired
    tournamentLock.acquire(threadID);

    //lock is aquired, spins for a random time
    cout << "Philo " << threadID << " is starting to eat." << endl;
    random_sleep();
    cout << "Philo " << threadID << " has finished eating." << endl;

    //philosopher releases the lock from top of tree down
    tournamentLock.release(threadID);
}

void fine_grained(int threadID, FineGrainedLock& fineGrainedLock){
    //philosopher thinks (can be done concurrently with other philosophers)
    //      This output will be intentionally garbled and overlap with other threads 
    //      because there is intentionally no mutex in place.
    cout << "Philo " << threadID << " has started thinking." << endl;
    random_sleep();
    cout << "Philo " << threadID << " has finished thinking." << endl;

    //philosopher eats (tries to reach for their adjacent chopsticks)
    fineGrainedLock.acquire(threadID);
    cout << "Philo " << threadID << " is starting to eat." << endl;
    random_sleep();
    cout << "Philo " << threadID << " has finished eating." << endl;

    //Releases lock on chopstick after finished eating
    fineGrainedLock.release(threadID);
}