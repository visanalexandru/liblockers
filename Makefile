.PHONY: bin library tests clean all

all : bin library tests

# Tests
tests: bin/test_spinlock bin/test_thread bin/test_semaphore bin/test_spinlock bin/test_mutex
bin/test_semaphore : bin/liblockers.a tests/test_semaphore.c
	gcc tests/test_semaphore.c -pthread -Ilib -Lbin -llockers -o bin/test_semaphore
bin/test_thread: bin/liblockers.a tests/test_thread.c
	gcc tests/test_thread.c -pthread -Ilib -Lbin -llockers -o bin/test_thread
bin/test_spinlock : bin/liblockers.a tests/test_spinlock.c
	gcc tests/test_spinlock.c -pthread -Ilib -Lbin -llockers -o bin/test_spinlock
bin/test_mutex: bin/liblockers.a tests/test_mutex.c
	gcc tests/test_mutex.c -pthread -Ilib -Lbin -llockers -o bin/test_mutex



# Library
library : bin/liblockers.a
bin/liblockers.a : bin/spinlock.o bin/thread.o bin/mutex.o bin/semaphore.o
	ar rcs bin/liblockers.a bin/spinlock.o bin/thread.o bin/mutex.o bin/semaphore.o
bin/spinlock.o: lib/spinlock.c lib/spinlock.h
	gcc -c lib/spinlock.c -o bin/spinlock.o
bin/thread.o : lib/thread.c lib/thread.h
	gcc -c lib/thread.c -o bin/thread.o
bin/mutex.o : lib/mutex.c lib/mutex.h
	gcc -c lib/mutex.c -o bin/mutex.o
bin/semaphore.o : lib/semaphore.c lib/semaphore.h
	gcc -c lib/semaphore.c -o bin/semaphore.o

bin:
	mkdir -p bin

clean:
	rm -r bin/