//
// Created by visan on 1/18/23.
//

#ifndef LOCKERS_MUTEX_H
#define LOCKERS_MUTEX_H

#include "thread.h"
#include "spinlock.h"
#include "signal.h"

#define MUTEX_LOCKED 0
#define MUTEX_UNLOCKED 1

/* The mutex must implement two methods: acquire() and release().
 * If the mutex is LOCKED, a call to acquire() succeeds and then
 * the lock is considered LOCKED. A thread that attempts to acquire
 * a LOCKED lock is blocked until the lock is released.
 * We implement this behaviour as follows: The mutex will hold
 * information about the threads that are waiting for the lock
 * to be UNLOCKED in a queue. Whenever a thread tries to acquire
 * a LOCKED lock, it is blocked and added into the queue of waiting threads.
 * When the thread that owns the lock releases the lock, the first thread
 * in the waiting queue is unblocked. All calls to acquire() and release()
 * must be made sequentially so a spinlock is used to ensure this.
 * */
typedef struct mutex {
    // The spinlock used to ensure sequential operations
    spinlock spinlock;

    // The list of waiting threads.
    thread_list thread_list;

    // The status of the mutex. Can be either MUTEX_LOCKED or MUTEX_UNLOCKED.
    int status;
} mutex;

/* Initializes the given mutex. The status will be UNLOCKED.*/
void mutex_init(mutex *mutex);

/* Frees the memory used by the given mutex. */
void mutex_delete(mutex *mutex);

/* Acquires the lock. If the mutex is LOCKED, block until it is UNLOCKED.*/
void mutex_acquire(mutex *mutex);

/* Releases the lock. Wake up the next thread in the waiting queue.*/
/* Note that this function does not check if the calling thread owns the lock.
 * Calling this function on a thread that does not own the lock is undefined. */
void mutex_release(mutex *mutex);


#endif //LOCKERS_MUTEX_H