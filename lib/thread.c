//
// Created by visan on 1/18/23.
//
#include"thread.h"

thread_info get_current_thread() {
    thread_info info;
    info.process_id = getpid();
    // There is no system call wrapper for gettid().
    info.thread_id = syscall(SYS_gettid);
    return info;
}

void thread_list_init(thread_list *list) {
    list->head = NULL;
    list->tail = NULL;
}

void thread_list_add(thread_list *list, thread_info info) {
    thread *t = malloc(sizeof(thread));

    t->next = NULL;
    t->info = info;

    // If the list is empty, set its head accordingly.
    if (thread_list_empty(list)) {
        list->head = t;
        list->tail = list->head;
    } else {
        list->tail->next = t;
        list->tail = t;
    }
}

thread_info thread_list_head(thread_list *list) {
    return list->head->info;
}

void thread_list_pop(thread_list *list) {
    thread *head = list->head;
    list->head = head->next;
    free(head);
}

int thread_list_empty(thread_list *list) {
    return list->head == NULL;
}

void thread_list_delete(thread_list *list) {
    thread *here = list->head;
    thread *next;
    while (here != NULL) {
        next = here->next;
        free(here);
        here = next;
    }
    list->head = NULL;
    list->tail = NULL;
}