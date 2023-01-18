//
// Created by visan on 1/18/23.
//

#ifndef LOCKERS_THREAD_LIST_H
#define LOCKERS_THREAD_LIST_H

#include<unistd.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<stdlib.h>

/* A thread is uniquely identified by the thread id and the process id.
 * This allows us to use the tgkill system call to signal a thread.
 */
typedef struct thread_info {
    // The process id of the thread.
    pid_t process_id;
    // The thread id of the thread.
    pid_t thread_id;
} thread_info;

typedef struct thread {
    // The thread information.
    struct thread_info info;
    // The next thread in the list.
    struct thread *next;
} thread;

/* This function returns information about the current thread. */
thread_info get_current_thread();

/*
 * A thread list is a data structure that implements the following operations
 * 1) Initialize an empty list.
 * 2) Add a thread to the tail of the list.
 * 3) Get and remove the head of the list.
 * 4) Check if the list is empty.
 * 5) Delete a list.
 */
typedef struct thread_list {
    struct thread *head;
    struct thread *tail;
} thread_list;

/* Initializes the given thread list.*/
void thread_list_init(thread_list *list);

/* Adds a new thread to the list.*/
void thread_list_add(thread_list *list, thread_info info);

/* Gets the head of the list. */
thread_info thread_list_head(thread_list *list);

/* Removes the head of the list.*/
void thread_list_pop(thread_list *list);

/* Returns 1 if the list is empty, otherwise 0. */
int thread_list_empty(thread_list *list);

/* Frees the memory allocated by the list.*/
void thread_list_delete(thread_list *list);

#endif //LOCKERS_THREAD_LIST_H
