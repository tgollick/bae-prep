CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -Iinclude

SRC      = $(wildcard src/*.c)
TEST_SRC = $(filter-out src/main.c, $(SRC)) $(wildcard tests/*.c)

state_machine: $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC)

run_tests: $(TEST_SRC)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRC)

.PHONY: test clean
test: run_tests
	./run_tests

clean:
	rm -f state_machine run_tests
