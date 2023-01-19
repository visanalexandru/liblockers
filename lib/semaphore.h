//
// Created by kath on 18.01.2023.
//

#include"spinlock.h"
#include"thread.h"
#include"signal.h"

#ifndef LOCKERS_SEMAPHORE_H
#define LOCKERS_SEMAPHORE_H


/* A semaphore must have a value that is greater or equal to 0. It must implement
 * two methods: wait() and signal(). If the semaphore's value is greater than 0,
 * a call to wait() simply decrements the value of the semaphore. A thread that
 * calls wait() on a semaphore that has a value of 0 is blocked until the semaphore's
 * value is greater than 0. We implement this behaviour as follows: The mutex will
 * hold information about the threads that are waiting for the semaphore to have a
 * value greater than 0 in a queue. Whenever a thread tries to wait() on a semaphore
 * that has a value of 0, it is blocked and added to the queue. A call to signal()
 * wakes up the first thread in the queue to resume execution if the queue is not empty.
 * If the queue is empty, simply increment the value of the semaphore.
 * All calls to wait() and signal() must be made sequentially so a spinlock is
 * used to ensure this.
 *  */

typedef struct semaphore {
    // The spinlock used to ensure sequential operations
    spinlock spinlock;

    // The list of waiting threads.
    thread_list thread_list;

    // The value of the semaphore.
    int value;
} semaphore;

// Initializes the semaphore with the given value.
void semaphore_init(semaphore *semaphore, int value);

// Frees the memory used by the semaphore.
void semaphore_destroy(semaphore *semaphore);

// Tries to decrement the value of the semaphore.
void semaphore_wait(semaphore *semaphore);

// Increments the value of the semaphore.
void semaphore_signal(semaphore *semaphore);

#endif
