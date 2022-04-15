
Writer: writer.o
	gcc -g -Wall writer.o -o Writer -lrt
	
writer.o: writer.c
	gcc -g -Wall -c writer.c -o writer.o -lrt

run: Writer
	clear
	./Writer

clean:
	rm writer.o Writer
