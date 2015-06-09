CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=gnu11 -fPIC
NAME=base36
NSSLIB=libnss_$(NAME).so.2

all: lib

lib: $(NAME).c
	$(CC) $(CFLAGS) -shared -Wl,-soname,"$(NSSLIB)" -o "$(NSSLIB)" $(NAME).c

clean:
	rm -f "$(NSSLIB)"

install: uninstall
	cp "$(NSSLIB)" /usr/lib/

uninstall:
	rm -rf "/usr/lib/$(NSSLIB)"
