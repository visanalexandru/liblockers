
.PHONY: bin library tests clean all

all : bin library tests

# Tests
tests: bin/test_spinlock
bin/test_spinlock : bin/liblockers.a tests/test_spinlock.c
	gcc tests/test_spinlock.c -pthread -Ilib -Lbin -llockers -o bin/test_spinlock

# Library
library : bin/liblockers.a
bin/liblockers.a : bin/spinlock.o
	ar rcs bin/liblockers.a bin/spinlock.o
bin/spinlock.o: lib/spinlock.c lib/spinlock.h
	gcc -c lib/spinlock.c -o bin/spinlock.o


bin:
	mkdir -p bin

clean:
	rm -r bin/