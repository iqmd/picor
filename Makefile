CFLAGS= -Wall -Wextra -std=c11  -Wpedantic -ggdb
LIBS= -lX11


picor: main.c
	$(CC) $(CFLAGS) -o picor main.c  $(LIBS)
