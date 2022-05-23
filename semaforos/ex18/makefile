main.o: main.c
	gcc -g -Wall main.c -o main.o -lrt -lpthread

run: main.o
	./main.o

clean:
	rm *.o
	rm /dev/shm/*