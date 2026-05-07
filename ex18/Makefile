CFLAGS = -Wall -g -O0 -no-pie
CC = cc

SOURCES = $(wildcard *.c)
TARGETS = $(patsubst %.c,%,$(SOURCES))

PROGRAM ?= ex18
VALGRIND = valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes

.PHONY: all clean run valgrind

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

run: $(PROGRAM)
	./$(PROGRAM)

valgrind: $(PROGRAM)
	$(VALGRIND) ./$(PROGRAM)

clean:
	rm -f $(TARGETS) *.o
