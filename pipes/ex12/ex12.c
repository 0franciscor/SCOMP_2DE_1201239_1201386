#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#define PRODUCT_LIST 4

struct request {
    int barcode;
    int childID;
};

struct product {
    int barcode;
    char name[20];
    float price;
};

int createChild(int productPipe[2]);
int numGenerator();

int main(){

    struct product productArray [PRODUCT_LIST];
    struct product productToAdd;
    
    productToAdd.barcode = 1;
    strcpy(productToAdd.name, "Oreos");
    productToAdd.price = 1;
    productArray[0] = productToAdd;

    productToAdd.barcode = 2;
    strcpy(productToAdd.name, "Banana");
    productToAdd.price = 0.5;
    productArray[1] = productToAdd;

    productToAdd.barcode = 3;
    strcpy(productToAdd.name, "Televisor FHD");
    productToAdd.price = 100;
    productArray[2] = productToAdd;

    productToAdd.barcode = 4;
    strcpy(productToAdd.name, "Produto Not Found");
    productToAdd.price = 0;
    productArray[3] = productToAdd;

    int productPipe[2];

    int requestPipe[2];

    if(pipe(requestPipe) == -1){
        printf("Error when creating Pipe");
        return -1;
    }

    int processIndex = createChild(productPipe);

    if(processIndex != -1){
        close(requestPipe[0]);
        close(productPipe[1]);
        
        
        for(;;){
            int requestedBarcode = numGenerator();
    
            struct request request;
            request.barcode = requestedBarcode;
            request.childID = processIndex;

            write(requestPipe[1], &request, sizeof(request));

            struct product requestedProduct;

            read(productPipe[0], &requestedProduct, sizeof(requestedProduct));

            printf("Requested Product - Barcode: %d PID: %d\n", requestedBarcode, getpid());            
            printf("Received Product - Barcode: %d Name: %s Price: %.2f\n\n", requestedProduct.barcode, requestedProduct.name, requestedProduct.price);

            sleep(1);
    
        }
        close(requestPipe[1]);
        close(productPipe[0]);
        return 0;
    }

    close(requestPipe[1]);
    close(productPipe[0]);

    struct request request;
    while(read(requestPipe[0], &request, sizeof(request)) != 0){
    
        struct product productToSend;

        for(int i = 0; i < PRODUCT_LIST; i++){
            if(request.barcode == productArray[i].barcode){
                productToSend = productArray[i];
                break;
            }
            productToSend = productArray[i];
        }

        write(productPipe[1], &productToSend, sizeof(productToSend));
    }

    close(requestPipe[0]);4
    close(productPipe[1]);

    return 0;
}

int createChild(int productPipe[2]){
    pid_t p;
    for(int i = 0; i < 5; i++){
        
        if(pipe(productPipe) == -1){
            printf("Error when creating Pipe");
            return -1;
        }  

        p = fork();
        if(p == 0)
            return i;
        sleep(2);
    }
    return -1;
}

int numGenerator(){
    srand(time(0));
    int lower = 1, upper = 5;
    int numToGuess = (rand() % (upper - lower + 1)) + lower;

    return numToGuess;
}