#ifndef PETERSON_H
#define PETERSON_H

#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <sched.h>
#include <time.h>

// Structure for Peterson's Lock
typedef struct {
    atomic_bool flag[2];
    atomic_int turn;
} PetersonLock;

// Shared sleep duration for waiting
extern struct timespec ts;

// Function prototypes
void initialize(PetersonLock *lock);
void acquire(PetersonLock *lock, int threadID);
void release(PetersonLock *lock, int threadID);

#endif // PETERSON_H
