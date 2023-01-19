//
// Created by kath on 18.01.2023.
//
#include "semaphore.h"
#include "thread.h"

void semaphore_init(semaphore *semaphore, int value) {
    // Initialize the thread list.
    thread_list_init(&semaphore->thread_list);

    // Initialize the spinlock.
    spinlock_init(&semaphore->spinlock);

    semaphore->value = value;
}

void semaphore_destroy(semaphore *semaphore) {
    // Delete the thread list.
    thread_list_delete(&semaphore->thread_list);

    // Delete the spinlock.
    spinlock_delete(&semaphore->spinlock);

    semaphore->value = 0;
}

void semaphore_wait(semaphore *semaphore) {
    spinlock_lock(&semaphore->spinlock);

    // If the semaphore is not 0, just decrement its value.
    if (semaphore->value != 0) {
        semaphore->value--;
        spinlock_unlock(&semaphore->spinlock);
    } else {

        // Get the current thread info.
        thread_info thread = get_current_thread();

        // Add it to the waiting list.
        thread_list_add(&semaphore->thread_list, thread);

        // Create the signal mask.
        sigset_t block_signal;
        sigemptyset(&block_signal);
        sigaddset(&block_signal, SIGUSR1);
        // Block the signal before calling sigwait().
        pthread_sigmask(SIG_BLOCK, &block_signal, NULL);

        spinlock_unlock(&semaphore->spinlock);

        // Go to sleep until we receive SIGUSR1.
        int received;
        sigwait(&block_signal, &received);
        // The thread can now continue.
    }
}

void semaphore_signal(semaphore *semaphore) {
    spinlock_lock(&semaphore->spinlock);

    // If the thread list is empty, increment the value of the semaphore.
    if (thread_list_empty(&semaphore->thread_list)) {
        semaphore->value++;
        spinlock_unlock(&semaphore->spinlock);
    } else {
        // Get the first thread in the queue.
        thread_info first = thread_list_head(&semaphore->thread_list);
        // Remove the head.
        thread_list_pop(&semaphore->thread_list);
        spinlock_unlock(&semaphore->spinlock);
        // Wake up the thread.
        pthread_kill(first.thread, SIGUSR1);
    }
}
