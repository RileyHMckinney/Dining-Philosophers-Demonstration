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

// Function executed by each philosopher (thread)
void coarse_grained(int threadID, TournamentTreeLock& tournamentLock);

void fine_grained(int threadID, FineGrainedLock& fineGrainedLock);

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

    cout << "|----------------------------------------------------------------------------|" << endl;
    cout << "| Simulate the Dining Philosophers problem using the coarse-grained approach.|" << endl;
    cout << "|----------------------------------------------------------------------------|" << endl;

    // Create the tournament lock with the specified number of threads
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

    threads.clear();

    cout << "|--------------------------------------------------------------------------|" << endl;
    cout << "| Simulate the Dining Philosophers problem using the fine-grained approach.|" << endl;
    cout << "|--------------------------------------------------------------------------|" << endl;

    FineGrainedLock fineGrainedLock(numThreads);

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

void random_sleep() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 500);
    this_thread::sleep_for(chrono::milliseconds(dist(gen)));
}

void coarse_grained(int threadID, TournamentTreeLock& tournamentLock) {
    //philosopher thinks (can be done concurrently with other philosophers)
    cout << "Philo " << threadID << " has started thinking." << endl;
    random_sleep();
    cout << "Philo " << threadID << " has finished thinking." << endl;

    //philosopher eats (can only be done one philosopher at a time)
    tournamentLock.acquire(threadID);
    cout << "Philo " << threadID << " is starting to eat." << endl;
    random_sleep();
    cout << "Philo " << threadID << " has finished eating." << endl;
    tournamentLock.release(threadID);
}

void fine_grained(int threadID, FineGrainedLock& fineGrainedLock){
    //philosopher thinks (can be done concurrently with other philosophers)
    cout << "Philo " << threadID << " has started thinking." << endl;
    random_sleep();
    cout << "Philo " << threadID << " has finished thinking." << endl;

    //philosopher eats (tries to reach for their adjacent chopsticks)
    fineGrainedLock.acquire(threadID);
    cout << "Philo " << threadID << " is starting to eat." << endl;
    random_sleep();
    cout << "Philo " << threadID << " has finished eating." << endl;
    fineGrainedLock.release(threadID);
}