//
// Created by visan on 1/19/23.
//

#include "rwlock.h"

void rwlock_init(rwlock *rwlock) {
    // Initialize the semaphore.
    semaphore_init(&rwlock->empty, 1);

    // Initialize the mutex.
    mutex_init(&rwlock->mutex);

    rwlock->num_readers = 0;
}

void rwlock_delete(rwlock *rwlock) {
    // Delete the semaphore.
    semaphore_delete(&rwlock->empty);

    // Delete the mutex.
    mutex_delete(&rwlock->mutex);

    rwlock->num_readers = 0;
}


void rwlock_read_acquire(rwlock *rwlock) {
    // wait()
    // signal()
    // Lock the read mutex to sync readers.
    mutex_acquire(&rwlock->mutex);
    if (rwlock->num_readers == 0) {
        // If this is the first reader, wait for the empty semaphore.
        // If the semaphore's value is not 1, then a writer is currently owning the lock.
        semaphore_wait(&rwlock->empty);
    }
    rwlock->num_readers++;
    mutex_release(&rwlock->mutex);
    // We can now continue to the reader section.
}

void rwlock_read_release(rwlock *rwlock) {
    // Lock the read mutex to sync readers.
    mutex_acquire(&rwlock->mutex);
    rwlock->num_readers--;
    if (rwlock->num_readers == 0) {
        // If there are no more readers left, signal the empty semaphore.
        semaphore_signal(&rwlock->empty);
    }
    mutex_release(&rwlock->mutex);
}

void rwlock_write_acquire(rwlock *rwlock) {
    // A thread can acquire the writer lock if there are no threads that
    // own read/write locks.
    // Wait until no more owning threads.
    semaphore_wait(&rwlock->empty);
    // We can then continue to the writer section.
}

void rwlock_write_release(rwlock *rwlock) {
    // Just signal the empty semaphore because there are no more owning threads.
    semaphore_signal(&rwlock->empty);
}