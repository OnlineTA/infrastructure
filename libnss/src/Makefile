CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99
NAME=onlineta
LIBNSS_NAME=libnss_$(NAME).so.2

all: lib

lib: main.c
	$(CC) $(CFLAGS) -shared -o $(LIBNSS_NAME) -Wl,-soname,$(LIBNSS_NAME) main.c

install:
