//
// Created by visan on 1/18/23.
//

#include"mutex.h"

void mutex_init(mutex *mutex) {
    // Initialize the thread list.
    thread_list_init(&mutex->thread_list);

    // Initialize the spinlock.
    spinlock_init(&mutex->spinlock);

    mutex->status = MUTEX_UNLOCKED;
}

void mutex_delete(mutex *mutex) {
    // Delete the thread list.
    thread_list_delete(&mutex->thread_list);

    // Delete the spinlock.
    spinlock_delete(&mutex->spinlock);

    mutex->status = MUTEX_UNLOCKED;
}

void mutex_acquire(mutex *mutex) {
    spinlock_lock(&mutex->spinlock);

    // If it is unlocked, just set the status to LOCKED.
    if (mutex->status == MUTEX_UNLOCKED) {
        mutex->status = MUTEX_LOCKED;
        spinlock_unlock(&mutex->spinlock);
    } else {
        // Get the current thread info.
        thread_info info = get_current_thread();
        // Add this thread to the queue.
        thread_list_add(&mutex->thread_list, info);

        //https://pubs.opengroup.org/onlinepubs/9699919799/functions/sigwait.html
        // The signal needs to be blocked at the time of the call to sigwait().
        // Create the signal mask.
        sigset_t block_signal;
        sigemptyset(&block_signal);
        sigaddset(&block_signal, SIGUSR1);
        // Block the signal before calling sigwait().
        pthread_sigmask(SIG_BLOCK, &block_signal, NULL);
        spinlock_unlock(&mutex->spinlock);

        // Put the thread to sleep until it receives SIGUSR1.
        int signal;
        sigwait(&block_signal, &signal);
        // The thread is then notified by the thread owning the mutex and continues.
    }
    // The thread can now enter the critical part.
}

void mutex_release(mutex *mutex) {
    spinlock_lock(&mutex->spinlock);
    // If the thread list is empty, set the status to UNLOCKED.
    if (thread_list_empty(&mutex->thread_list)) {
        mutex->status = MUTEX_UNLOCKED;
        spinlock_unlock(&mutex->spinlock);
    } else {
        // Else, get the first thread in the waiting queue and wake it up.
        thread_info first = thread_list_head(&mutex->thread_list);
        // Remove the head of the list.
        thread_list_pop(&mutex->thread_list);
        spinlock_unlock(&mutex->spinlock);
        // Wake the thread up.
        pthread_kill(first.thread, SIGUSR1);
    }
}


