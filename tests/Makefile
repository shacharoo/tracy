all: main

main: ../src/tracy.h ../src/tracy.c main.c
	$(CC) main.c ../src/tracy.c -o main -W -Wall -Werror -I../src

clean:
	rm -f main *.o

.PHONY: clean all
