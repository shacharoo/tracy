all: main

main: tracy.h tracy.c main.c
	$(CC) main.c tracy.c -o main -W -Wall -Werror

clean:
	rm -f main *.o

.PHONY: clean all
