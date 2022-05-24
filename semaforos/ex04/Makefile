main: main.c
	gcc -g -Wall main.c -o main.o -lrt -lpthread

run: main
	./main.o

clean:
	rm *.o
	rm /dev/shm/*