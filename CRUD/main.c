#include <stdio.h>
#include "Libs\crudHandler.h"

int main(void) {
    FILE *myFile;
    Product newProduct;
    /*
        r - open a file in read mode
        w - opens or create a text file in write mode
        a - opens a file in append mode
        r+ - opens a file in both read and write mode
        a+ - opens a file in both read and write mode
        w+ - opens a file in both read and write mode
    */

    newProduct.name = "Fried Rice";
    newProduct.id = 8;
    newProduct.price = 49.99;

    myFile = fopen("Data/MockUpProduct.csv", "a+");

    printf("%d\n", deleteProduct(myFile, "Pad Thai"));

    fclose(myFile);

    return 0;
}
