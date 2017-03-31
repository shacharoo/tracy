tracy.o: tracy.h tracy.c
	gcc -c tracy.c -o tracy.o

main: main.c tracy.o
	gcc main.c tracy.o -o main
