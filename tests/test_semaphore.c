//
// Created by kath on 18.01.2023.
//
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<assert.h>

#define NUM_THREADS 50
#define NUM_ITERATIONS 100000

int global_counter;
semaphore semaphore1;

// Fuzz test the mutex implementation.
void *thread_func(void *args) {
    semaphore_wait(&semaphore1);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        global_counter++;
    }
    semaphore_signal(&semaphore1);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    semaphore_init(&semaphore1, 1);

    // Create the threads.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, NULL);
    }
    // Join the threads.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    semaphore_destroy(&semaphore1);
    assert(global_counter == NUM_ITERATIONS * NUM_THREADS);

    return 0;
}