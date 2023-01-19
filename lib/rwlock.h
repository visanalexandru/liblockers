//
// Created by visan on 1/19/23.
//

#ifndef LOCKERS_RWLOK_H
#define LOCKERS_RWLOK_H

#include"mutex.h"
#include"semaphore.h"

/* The rwlock must implement 4 functions: acquire_read(), acquire_write(), release_read()
 * and release_write(). A call to acquire_read() will block the current thread if
 * there is a thread currently owning a write lock or waiting to acquire a write lock.
 * A call to acquire_write() will block the current thread if there are any threads that
 * own read locks or if there is a thread that owns a write lock.
 * Calling release_read() will unblock threads that are waiting to acquire a write lock.
 * Calling release_write() will unblock threads that are waiting to acquire a write lock or a
 * read lock.
 *
 * The rwlock is used when we can allow multiple threads to read a resource concurrently and
 * allow only one thread to hold the lock when data is modified.
 */

typedef struct rwlock {
    // The number of threads that own a read lock.
    int num_readers;

    // A mutex that synchronizes access to the read lock.
    mutex mutex;

    // This semaphore is used to signal if there is no thread currently owning
    // any type of lock.
    semaphore empty;

    //this semaphore is used to a turnstile for readers and a mutex for writers.
    semaphore turnstile;
} rwlock;


// Initializes the rwlock.
void rwlock_init(rwlock *rwlock);

// Deletes the rwlock.
void rwlock_delete(rwlock *rwlock);

// Acquires the read lock.
void rwlock_read_acquire(rwlock *rwlock);

// Releases the read lock.
void rwlock_read_release(rwlock *rwlock);

// Acquires the write lock.
void rwlock_write_acquire(rwlock *rwlock);

// Releases the write lock.
void rwlock_write_release(rwlock *rwlock);


#endif //LOCKERS_RWLOK_H
