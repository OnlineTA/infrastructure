CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=gnu11
LIB=liblolsegfault.so.2

all: lib

lib: main.c
	$(CC) $(CFLAGS) -shared -o $(LIB) -Wl,-soname,$(LIB) main.c

install:
	cp $(LIB) /lib64/
