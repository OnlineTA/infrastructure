CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=gnu11 -fPIC
NAME=onlineta
NSSLIB=libnss_$(NAME).so.2

all: lib

lib: main.c
	$(CC) $(CFLAGS) -shared -o $(NSSLIB) -Wl,-soname,$(NSSLIB) main.c

install:
	cp $(NSSLIB) /lib64/
