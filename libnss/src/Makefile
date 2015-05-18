CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=gnu11 -fPIC
NAME=base36
NSSLIB=libnss_$(NAME).so.2

all: lib

lib: $(NAME).c
	$(CC) $(CFLAGS) -shared -o $(NSSLIB) -Wl,-soname,$(NSSLIB) $(NAME).c

install: uninstall
	cp $(NSSLIB) /lib64/

uninstall:
	rm -rf /lib64/$(NSSLIB)
