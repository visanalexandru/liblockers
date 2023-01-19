//
// Created by visan on 1/19/23.
//

#include <semaphore.h>
#include<mutex.h>
#include<stdio.h>
#include<assert.h>

#define CONSUMER_CAN_CONSUME 10
#define PRODUCER_CAN_PRODUCE 50
#define NUM_PRODUCERS 300
#define NUM_CONSUMERS 500
#define BUFFER_SIZE NUM_PRODUCERS * PRODUCER_CAN_PRODUCE

int buffer[BUFFER_SIZE];
// Number of items in the buffer.
int items = 0;

// Total number of items produced.
int produced = 0;

// Total number of items consumed.
int consumed = 0;

semaphore full, empty;
mutex lock;


void *produce(void *arg) {
    for (int i = 0; i < PRODUCER_CAN_PRODUCE; i++) {
        // Produce an item.
        int item = rand() % 100;

        // Wait for an empty slot.
        semaphore_wait(&empty);

        mutex_acquire(&lock);
        // Add a new item.
        printf("Produce %d: %d \n", *((int *) arg), item);
        buffer[items] = item;
        items++;
        produced++;
        mutex_release(&lock);
        // Signal a new item.
        semaphore_signal(&full);
    }
    return NULL;
}

void *consume(void *arg) {
    for (int i = 0; i < CONSUMER_CAN_CONSUME; i++) {
        // Wait for an item.
        semaphore_wait(&full);
        // Consume an item.
        mutex_acquire(&lock);
        int to_consume = buffer[items - 1];
        items--;
        consumed++;
        mutex_release(&lock);
        // Signal a new item.
        semaphore_signal(&empty);

        printf("Consume %d: %d \n", *((int *) arg), to_consume);
    }
    return NULL;
}


int main() {
    int producer_id[NUM_PRODUCERS];
    int consumer_id[NUM_CONSUMERS];
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    semaphore_init(&full, 0);
    semaphore_init(&empty, BUFFER_SIZE);
    mutex_init(&lock);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_id[i] = i;
        pthread_create(&producers[i], NULL, produce, &producer_id[i]);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_id[i] = i;
        pthread_create(&consumers[i], NULL, consume, &consumer_id[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }
    assert(produced == NUM_PRODUCERS * PRODUCER_CAN_PRODUCE);
    assert(consumed == NUM_CONSUMERS * CONSUMER_CAN_CONSUME);

    return 0;
}