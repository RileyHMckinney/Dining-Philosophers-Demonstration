#include "peterson.h"

struct timespec ts = {0, 1000}; 

typedef struct {
    atomic_bool flag[2];
    atomic_int turn;
} PetersonLock;

void initialize(PetersonLock *lock){
    atomic_store(&lock->flag[0], false);
    atomic_store(&lock->flag[1], false);
    atomic_store(&lock->turn, 0);
}

void aquire(PetersonLock *lock, int threadID){
    int other = 1 - threadID;
    atomic_store(&lock->flag[threadID], true);
    atomic_store(&lock->turn, 1); 

    while(atomic_load(&lock->flag[threadID]) && atomic_load(&lock->turn) == other){
        nanosleep(&ts, NULL);
    }
}

void release(PetersonLock *lock, int threadID){
    atomic_store(&lock->flag[threadID], false);
}