//
// Created by visan on 1/18/23.
//

#include<stdio.h>
#include<thread.h>
#include<assert.h>

void test_current_thread() {
    thread_info info = get_current_thread();
    assert(getpid() == info.process_id);
    assert(syscall(SYS_gettid) == info.thread_id);
}

void test_list_insert() {
    thread_list list;
    thread_list_init(&list);
    assert(thread_list_empty(&list));

    thread_info info[10];
    // Add 10 threads to the list.
    for (int i = 0; i < 10; i++) {
        info[i].thread_id = i;
        info[i].process_id = i * 10;
        thread_list_add(&list, info[i]);
    }
    // The list must not be empty.
    assert(!thread_list_empty(&list));
    // The head of the list is the first thread.
    assert(thread_list_head(&list).process_id == info[0].process_id &&
           thread_list_head(&list).thread_id == info[0].thread_id);
    // Remove the head.
    thread_list_pop(&list);
    assert(!thread_list_empty(&list));
    // The head of the list is now the second thread.
    assert(thread_list_head(&list).process_id == info[1].process_id &&
           thread_list_head(&list).thread_id == info[1].thread_id);
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
        info.process_id = 2 * i;
        info.thread_id = 3 * i;
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