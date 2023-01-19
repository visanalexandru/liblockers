//
// Created by visan on 1/19/23.
//
#include<semaphore.h>
#include<mutex.h>
#include<rwlock.h>
#include<string.h>
#include<assert.h>
#include<stdio.h>

#define BUFFER_SIZE 1000000

#define NUM_READERS 50
#define NUM_WRITERS 50

rwlock rw;
mutex mt;
int data[BUFFER_SIZE];
// The number of writers at a given moment.
int num_writers;

void *reader(void *arg) {
    rwlock_read_acquire(&rw);
    printf("Reader enters %d\n", *(int *) arg);

    int *old_data = malloc(BUFFER_SIZE * sizeof(int));
    memcpy(old_data, data, BUFFER_SIZE * sizeof(int));
    // Check if the data has changed when we were reading.
    assert(memcmp(data, old_data, BUFFER_SIZE * sizeof(int)) == 0);

    printf("Reader leaves %d \n", *(int *) arg);
    rwlock_read_release(&rw);
    return NULL;
}

void *writer(void *arg) {
    rwlock_write_acquire(&rw);
    printf("Writer enters %d\n", *(int *) arg);

    mutex_acquire(&mt);
    num_writers++;
    mutex_release(&mt);

    // Change data.
    for (int i = 0; i < BUFFER_SIZE; i++) {
        data[i] = rand() % 1000;
        mutex_acquire(&mt);
        // Check if there is another writer writing.
        assert(num_writers == 1);
        mutex_release(&mt);
    }
    mutex_acquire(&mt);
    num_writers--;
    mutex_release(&mt);

    printf("Writer leaves %d\n", *(int *) arg);
    rwlock_write_release(&rw);
    return NULL;
}


int main() {
//    rwlock_init(&rw);
//    mutex_init(&mt);
//
//    pthread_t readers[NUM_READERS];
//    pthread_t writers[NUM_WRITERS];
//
//    int reader_id[NUM_READERS];
//    int writer_id[NUM_WRITERS];
//    for (int i = 0; i < NUM_READERS; i++) {
//        reader_id[i] = i;
//        pthread_create(&readers[i], NULL, reader, &reader_id[i]);
//    }
//    for (int i = 0; i < NUM_WRITERS; i++) {
//        writer_id[i] = i;
//        pthread_create(&writers[i], NULL, writer, &writer_id[i]);
//    }
//
//
//    for (int i = 0; i < NUM_READERS; i++) {
//        pthread_join(readers[i], NULL);
//    }
//    for (int i = 0; i < NUM_WRITERS; i++) {
//        pthread_join(writers[i], NULL);
//    }
//
//    mutex_delete(&mt);
//    rwlock_delete(&rw);


    rwlock_init(&rw);
    mutex_init(&mt);

    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];

    int reader_id[NUM_READERS];
    int writer_id[NUM_WRITERS];
    for (int i = 0; i < NUM_READERS/2; i++) {
        reader_id[i] = i;
        pthread_create(&readers[i], NULL, reader, &reader_id[i]);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_id[i] = i;
        pthread_create(&writers[i], NULL, writer, &writer_id[i]);
    }
    for (int i = NUM_READERS/2; i < NUM_READERS; i++) {
            reader_id[i] = i;
            pthread_create(&readers[i], NULL, reader, &reader_id[i]);
    }
    for (int i = NUM_WRITERS/2; i < NUM_WRITERS; i++) {
        writer_id[i] = i;
        pthread_create(&writers[i], NULL, writer, &writer_id[i]);
    }
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    mutex_delete(&mt);
    rwlock_delete(&rw);

    return 0;
}