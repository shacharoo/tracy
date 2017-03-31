tracy.o: tracy.h tracy.c
	g++ -c tracy.c -o tracy.o

main: main.c tracy.o
	g++ main.c tracy.o -o main
