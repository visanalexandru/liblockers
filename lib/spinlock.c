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
    while (1) {
        // Try for NUM_TRIES to acquire the lock.
        for (int i = 0; i < NUM_TRIES; i++) {
            if (COMPARE_AND_SWAP(&spinlock->status, SPINLOCK_UNLOCKED, SPINLOCK_LOCKED) == SPINLOCK_UNLOCKED) {
                return;
            }
        }
        // If we couldn't acquire the lock, yield the cpu.
        sched_yield();
    }
}

void spinlock_unlock(spinlock *spinlock) {
    spinlock->status = SPINLOCK_UNLOCKED;
}