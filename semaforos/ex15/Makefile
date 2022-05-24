prog_sem: main.o
	gcc main.o -o prog_sem -lrt -lpthread

main.o: main.c
	gcc -g -Wall -c main.c -o main.o

clean:
	rm -f *.o prog_sem

run: prog_sem
	./prog_sem
