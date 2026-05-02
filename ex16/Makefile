CFLAGS = -Wall -g
CC = cc

SOURCES = $(wildcard *.c)
TARGETS = $(patsubst %.c,%,$(SOURCES))

PROGRAM ?= ex16
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