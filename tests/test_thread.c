//
// Created by visan on 1/18/23.
//

#include<stdio.h>
#include<thread.h>
#include<assert.h>

void test_current_thread() {
    thread_info info = get_current_thread();
    assert(pthread_self() == info.thread);
}

void test_list_insert() {
    thread_list list;
    thread_list_init(&list);
    assert(thread_list_empty(&list));

    thread_info info[10];
    // Add 10 threads to the list.
    for (int i = 0; i < 10; i++) {
        info[i].thread = i;
        thread_list_add(&list, info[i]);
    }
    // The list must not be empty.
    assert(!thread_list_empty(&list));
    // The head of the list is the first thread.
    assert(thread_list_head(&list).thread == info[0].thread);
    // Remove the head.
    thread_list_pop(&list);
    assert(!thread_list_empty(&list));
    // The head of the list is now the second thread.
    assert(thread_list_head(&list).thread == info[1].thread);
    for (int i = 0; i < 9; i++) {
        thread_list_pop(&list);
    }
    assert(thread_list_empty(&list));
}

void test_list_delete() {
    thread_list list;
    thread_list_init(&list);

    // Add 10 threads to the list.
    for (int i = 0; i < 10; i++) {
        thread_info info;
        info.thread = 3 * i;
        thread_list_add(&list, info);
    }
    thread_list_delete(&list);
    assert(thread_list_empty(&list));

}

int main() {
    test_current_thread();
    test_list_insert();
    test_list_delete();
    return 0;
}