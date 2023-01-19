//
// Created by kath on 18.01.2023.
//

#include"spinlock.h"
#include"thread.h"

#ifndef SEMAPHORE_H
#define SEMAPHORE_H


// A semaphore is a type of lock that works using two atomic instructions:
// signal() will increment the value of the semaphore.
// wait() will block the thread until the value of the semaphore is greater
// than 0. Once the value is greater than 0, the value will be decremented
// and the thread will be unblocked.



// This structure manages a semaphore.
// value - the value of the semaphore.
// thread_list - the list of threads that are waiting to aquire the lock.
// lock - a spinlock used to make wait() and signal() atomic.
typedef struct semaphore {
    int value;
    thread_list wait_list;
    spinlock lock;
} semaphore;


// When the value of the semaphore is 0, threads that
// call wait() are added into the list of waiting threads.
// When a call to signal() occurs, waiting threads are chosen
// for execution in the FIFO (first-in-first out) order.


// Initializes the semaphore with the given value.
void semaphore_init(semaphore *semaphore1, int value);

// Destroys the semaphore.
void semaphore_destroy(semaphore *semaphore1);

// Waits until the thread can aquire the semapore.
void semaphore_wait(semaphore *semaphore1);

// Increments the value of the semaphore.
void semaphore_signal(semaphore *semaphore1);

#endif
