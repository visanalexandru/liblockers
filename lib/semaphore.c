//
// Created by kath on 18.01.2023.
//
#include "semaphore.h"
#include "thread.h"

void semaphore_init(semaphore *semaphore1, int value) {
    semaphore1->value = value;

    //The list of waiting threads
    thread_list_init(&semaphore1->wait_list);

    //Initialization for the spinlock of the semaphore
    spinlock_init(&semaphore1->lock)
}

//Free memory used by semaphore
void semaphore_destroy(semaphore *semaphore1) {
    semaphore1->value = 0;
    thread_list_delete(&semaphore1->wait_list);
    spinlock_delete(&semaphore1->lock);
}

void semaphore_wait(semaphore *semaphore1){
    spinlock_lock(&semaphore1->lock);
    // If the value is 0, make the thread go to sleep
    // and add it to the list of waiting threads.
    if(semaphore1->value==0){
        thread * this = malloc(sizeof(struct thread));

        this->info.thread_id = gettid();
        this->next = NULL;

        // Add it to the waiting list.
        thread_list_add(&semaphore1->wait_list,this.info);

        // Initialize the signal set.
        sigset_t block_signals;
        sigemptyset(&block_signals);
        sigaddset(&block_signals, SIGUSR1);

        // Block the signals.
        sigprocmask(SIG_BLOCK, &block_signals, NULL);

        // We block the signal before unlocking the
        // spinlock to avoid a race condition.
        spinlock_unlock(&semaphore1->lock);

        // Go to sleep until we receive SIGUSR1.
        int received;
        sigwait(&block_signals,&received);
    }
    else{
        semaphore1->value--;
        spinlock_unlock(&semaphore1->lock);
    }
}

void semaphore_signal(semaphore *semaphore1) {
    spinlock_lock(&semaphore1->lock);

    //The next thread that is waiting will be chosen
    if (!thread_list_empty(&semaphore1->wait_list)) {
//        thread_info next_thr = thread_list_head(&semaphore1->wait_list);
        spinlock_unlock(&semaphore1->lock);

//        tgkill(next_thr->thread_id, SIGUSR1);
//        free(next_thr);
    } else {
        semaphore1->value++;
        spinlock_unlock(&semaphore1->lock);
    }
}
