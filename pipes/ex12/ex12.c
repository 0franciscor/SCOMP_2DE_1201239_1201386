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

int createChild(){
    pid_t p;
    for(int i = 1; i <= 5; i++){
        p = fork();
        if(p == 0)
            return i;
        //sleep(2);
    }
    return -1;
}

int numGenerator(){
    srand(time(0));
    int lower = 1, upper = 5;
    int numToGuess = (rand() % (upper - lower + 1)) + lower;

    return numToGuess;
}

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

    int cPipe[6][2]; //cPipe[0] é o pipe para fazer requests

    for(int i = 0; i < 6; i++){
        if(pipe(cPipe[i]) == -1){
            printf("Error when creating Pipe\n");
            return -1;
        }
    }
    
     // fecho de leitura de pipe comum escrita
    int processIndex = createChild();

    if(processIndex != -1){
        
        for(int i = 1; i < 6; i++){
            if(processIndex != i){
                close(cPipe[i][0]);
                close(cPipe[i][1]);
            }
        }

        close(cPipe[0][0]);    
        close(cPipe[processIndex][1]); // fecho de ecrita de pipe individual
        
        for(;;){
            int requestedBarcode = numGenerator();
    
            struct request request;
            request.barcode = requestedBarcode;
            request.childID = processIndex;

            write(cPipe[0][1], &request, sizeof(request));

            struct product requestedProduct;

            read(cPipe[processIndex][0], &requestedProduct, sizeof(requestedProduct));

            printf("Requested Product - Barcode: %d PID: %d\n", requestedBarcode, getpid());            
            printf("Received Product - Barcode: %d Name: %s Price: %.2f\n\n", requestedProduct.barcode, requestedProduct.name, requestedProduct.price);

            sleep(2);
    
        }
        close(cPipe[0][1]);
        close(cPipe[processIndex][0]);
        
        return 0;
    }

    close(cPipe[0][1]);
    for(int i = 1; i < 6; i++){
        close(cPipe[i][0]);
    }

    struct request request;

    while(read(cPipe[0][0], &request, sizeof(request)) != 0){
        
        int processIndex = request.childID;
        close(cPipe[processIndex][0]);
        
        struct product productToSend;

        for(int i = 0; i < PRODUCT_LIST; i++){
            if(request.barcode == productArray[i].barcode){
                productToSend = productArray[i];
                break;
            }
            productToSend = productArray[i];
        }

        write(cPipe[processIndex][1], &productToSend, sizeof(productToSend));
        sleep(1);
    }

    close(cPipe[0][0]);
    for(int i = 1; i < 6; i++){
        close(cPipe[i][1]);
    }

    return 0;
}