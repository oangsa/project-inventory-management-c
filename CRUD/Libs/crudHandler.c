#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "crudHandler.h"
#include "utils.h"

void printProduct(FILE *file) {
    char *name = (char*) malloc(99 * sizeof(char));
    char *description = (char*) malloc(999 * sizeof(char));
    float price;
    int id;

    printf("%6s%16s%6s%6s");

    while (!feof(file)) {
        fscanf(file, "%d,%6[^,],%f\n", &id, name, &price);
    }
}

int createProduct(FILE *file, Product product) {
    /*
        Use to create a product

        return 1 if the process is fail
        return 0 if the process success
    */

    // Error handling
    if (ferror(file)) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    fprintf(file, "%d,%s,%.2f", product.id, product.name, product.price);
    return 0;

}

int deleteProduct(FILE *file, char* productName) {
    /*
        Delete product by given the name

        return 1 if the process is fail
        return 0 if the process success

    */

    typedef struct tmp {
        char fcol[99];
        char scol[99];
        char thcol[99];
    } tmp;

    char name[99];
    float price;
    int id;
    int i = 0;
    int flag = 0;
    tmp test;

    Product products[999];

    FILE *tmpFile = fopen("Data/__MockUpProduct.csv", "w+");

    if (tmpFile == NULL) {
        printf("Cant open file\n");
        return 1;
    }

    if (ferror(file)) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    fscanf(file, "%[^,],%[^,],%[^,\n]", test.fcol, test.scol, test.thcol);

    while (!feof(file)) {
        fscanf(file, "%d,%[^,],%f\n", &id, name, &price);

        if (!strcmp(name, productName)) {
            flag = 1;
            continue;
        }

        products[i].id = id;
        products[i].name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'
        products[i].price = price;

        // printf("%d %s %.2f\n", products[i].id, products[i].name, products[i].price);

        i++;
    }

    if (!flag) {
        printf("Product named '%s' doesn't exist.\n", productName);
        return 1;
    }

    fprintf(tmpFile, "%s,%s,%s\n", test.fcol, test.scol, test.thcol);

    for (int j = 0; j < i; j++) {
        fprintf(tmpFile, "%d,%s,%.2f\n", j+1, products[j].name, products[j].price);
    }

    fclose(file);
    fclose(tmpFile);

    remove("Data/MockUpProduct.csv");
    rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

    return 0;
}
