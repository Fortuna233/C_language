CFLAGS=-Wall -g

SOURCES=$(wildcard *.c)

TARGETS=$(patsubst %.c,%.o,$(SOURCES))

all: $(TARGETS)
%.o: %.c
	cc $(CFLAGS) $< -o $@


clean:
	rm -f $(TARGETS) *.o

