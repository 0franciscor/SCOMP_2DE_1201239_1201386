bridge.o: bridge.c
	gcc -g -Wall bridge.c -o bridge.o -lrt -lpthread

car.o: car.c
	gcc -g -Wall car.c -o car.o -lrt -lpthread

bridge: bridge.o
	./bridge.o

car: car.o
	./car.o

clean:
	rm *.o
	rm /dev/shm/*
