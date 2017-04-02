tracy.o: tracy.h tracy.c
	gcc -c tracy.c -o tracy.o -W -Wall -Werror

main: main.c tracy.o
	gcc main.c tracy.o -o main -W -Wall -Werror

all: main

clean:
	rm -f main *.o

.PHONY: clean all
