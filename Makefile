CC?=gcc
O_LVL?=3

CFLAGS=-O$(O_LVL)

BINARIES= \
	server \
	client

all: $(BINARIES)

$(BINARIES): %: %.c
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm $(BINARIES)

.PHONY: all clean
