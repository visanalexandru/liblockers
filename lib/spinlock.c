//
// Created by visan on 1/17/23.
//
#include"spinlock.h"

void spinlock_init(spinlock *spinlock) {
    spinlock->status = SPINLOCK_UNLOCKED;
}

void spinlock_delete(spinlock *spinlock) {
    spinlock->status = SPINLOCK_UNLOCKED;
}

void spinlock_lock(spinlock *spinlock) {
    // If the status is unlocked, set the status to locked and stop the loop.
    // Else, if the status is locked, continue looping.
    while (COMPARE_AND_SWAP(&spinlock->status, SPINLOCK_UNLOCKED, SPINLOCK_LOCKED) == SPINLOCK_LOCKED);
}

void spinlock_unlock(spinlock *spinlock) {
    spinlock->status = SPINLOCK_UNLOCKED;
}