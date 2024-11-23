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

    newProduct.name = "Instant Noodle";
    // newProduct.price = 99.99;
    // newProduct.remain = 99;

    myFile = fopen("Data/MockUpProduct.csv", "a+");

    // if (createProduct(myFile, newProduct)) {
    //     printf("There's error during the product creation...\n");

    //     return 1;
    // }

    if (updateProduct(myFile, "Pad Krapao Moo", newProduct)) {
        printf("There's error during the product creation...\n");

        return 1;
    }

    // printProduct(myFile);

    // printf("%d\n", createProduct(myFile, newProduct));

    fclose(myFile);

    return 0;
}
