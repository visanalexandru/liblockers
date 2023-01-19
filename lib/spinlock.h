//
// Created by visan on 1/17/23.
//

#ifndef LOCKERS_SPINLOCK_H
#define LOCKERS_SPINLOCK_H

#include<sched.h>
/*
 * This macro defines the compare and swap function.
 * The CAS function takes a pointer to a value. If the value was old_value, set the value
 * to new_value and return true. Else, return false.
 * The function will always be executed in sequence.
 */
#define COMPARE_AND_SWAP(ptr, old_value, new_value) __sync_val_compare_and_swap(ptr,old_value,new_value)

// The number of unsuccessful tries to acquire the lock before yielding the cpu.
#define NUM_TRIES 100
#define SPINLOCK_LOCKED 1
#define SPINLOCK_UNLOCKED 0

/* The spinlock is a naive implementation of a mutex lock that implements busy waiting
 * as a method of waiting until the lock is unlocked.
 */
typedef struct spinlock {
    // The status can be either SPINLOCK_LOCKED or SPINLOCK_UNLOCKED
    int status;
} spinlock;


// Initializes the given spinlock.
void spinlock_init(spinlock *spinlock);

// Deletes the given spinlock.
void spinlock_delete(spinlock *spinlock);

/* This will try to acquire the lock by setting the status to LOCKED.
 * If the status is already locked, the function will retry to acquire the lock in a loop.
 */
void spinlock_lock(spinlock *spinlock);

/* This will set the spinlock status to UNLOCKED. This function should be called only
 * by the thread holding the lock.
 */
void spinlock_unlock(spinlock *spinlock);

#endif //LOCKERS_SPINLOCK_H
