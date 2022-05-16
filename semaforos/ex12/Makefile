seller: seller.c
	gcc -g -Wall seller.c -o seller.o -lrt -lpthread

client: client.c
	gcc -g -Wall client.c -o client.o -lrt -lpthread

runs: seller
	./seller.o

runc: client
	./client.o

clean:
	rm *.o main
