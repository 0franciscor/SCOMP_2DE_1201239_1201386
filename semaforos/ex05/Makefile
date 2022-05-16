main: main.o
	gcc -Wall -o main main.c -lpthread -lrt

main.o: main.c
	gcc -Wall -g -c main.c

run: main
	./main &

clean:
	rm *.o main
