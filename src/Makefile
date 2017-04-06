all: c cpp

c: tracy.h tracy.c main.c
	gcc main.c tracy.c -o c -W -Wall -Werror

cpp: tracy.h tracy.c main.c
	g++ main.c tracy.c -o cpp -W -Wall -Werror

clean:
	rm -f c cpp *.o

.PHONY: clean all
