//
// Created by visan on 1/17/23.
//

#include<stdio.h>
#include<pthread.h>
#include<spinlock.h>
#include<assert.h>

#define NUM_THREADS 50
#define NUM_ITERATIONS 100000

int global_counter;
spinlock lock;

// Fuzz test the mutex implementation.
void *thread_func(void *args) {
    spinlock_lock(&lock);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        global_counter++;
    }
    spinlock_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    spinlock_init(&lock);

    // Create the threads.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, NULL);
    }
    // Join the threads.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    spinlock_delete(&lock);
    assert(global_counter == NUM_ITERATIONS * NUM_THREADS);
    return 0;
}