reader.o: reader.c
	gcc -g -Wall reader.c -o reader.o -lrt -lpthread

writer.o: writer.c
	gcc -g -Wall writer.c -o writer.o -lrt -lpthread

main.o: main.c
	gcc -g -Wall main.c -o main.o -lrt -lpthread

reader: reader.o
	./reader.o

writer: writer.o
	./writer.o

main: main.o
	./main.o

clean:
	rm *.o
	rm /dev/shm/*
