#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "crudHandler.h"
#include "utils.h"

// Just structure of the header to prevent the confusion.
typedef struct Header {
    char no[99];
    char name[99];
    char price[99];
    char remain[99];
} Header;

void printProduct(FILE *file) {
    char name[99];
    float price;
    int id, remain;

    Header test;

    fscanf(file, "%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain);
    printf("-----------------------[Products]------------------------\n");
    printf("%-6s%-30s%-15s%-6s\n", test.no, test.name, test.price, test.remain);
    printf("%-6s%-30s%-15s%-6s\n", "--", "----", "-----", "------");

    while (!feof(file)) {
        fscanf(file, "%d,%[^,],%f,%d\n", &id, name, &price, &remain);

        printf("%-6d%-30s%-7.2f%-8s%-6d\n", id, name, price, "THB", remain);
    }
}

int createProduct(FILE *file, Product product) {
    /*
        Use to create a product

        return 1 if the process is fail
        return 0 if the process success
    */

    char name[99];
    float price;
    int id, remain;

    Header test;
    // Error handling
    if (ferror(file)) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    // Handling edge cases where user havent provide the product information.
    if (!product.name || !product.price || !product.remain) {
        printf("No enough information\n");

        return 1;
    }

    // Get rid the header
    fscanf(file, "%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain);

    // Loop checking if the provided product is already exist.
    while (!feof(file)) {
        fscanf(file, "%d,%[^,],%f,%d\n", &id, name, &price, &remain);

        if (!strcmp(name, product.name)) {
            printf("Product named '%s' is already exist!\n", product.name);
            return 1;
        }
    }

    fprintf(file, "%d,%s,%.2f,%d\n", id + 1, product.name, product.price, product.remain);

    return 0;
}

int deleteProduct(FILE *file, char* productName) {
    /*
        Delete product by given the name

        return 1 if the process is fail
        return 0 if the process success

    */

    char name[99];
    float price;
    int id, remain;
    int i = 0, flag = 0;

    Header test;

    Product products[999];

    FILE *tmpFile = fopen("Data/__MockUpProduct.csv", "w+");

    // Checking if unable to open file.
    if (tmpFile == NULL) {
        printf("Cant open file\n");
        return 1;
    }

    if (ferror(file)) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    // Get rid of the header
    fscanf(file, "%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain);

    // Loop until EOF
    while (!feof(file)) {
        fscanf(file, "%d,%[^,],%f,%d\n", &id, name, &price, &remain);

        // If the product in the current location is the same as the given product
        // If yes, just skip the part where putting the data in the array.
        if (!strcmp(name, productName)) {
            flag = 1;
            continue;
        }

        products[i].id = id;
        products[i].name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'
        products[i].price = price;
        products[i].remain = remain;

        i++;
    }

    // Checking if the product exist.
    if (!flag) {
        printf("Product named '%s' doesn't exist.\n", productName);
        remove("Data/__MockUpProduct.csv");
        return 1;
    }

    // Write data in the temporaly file.
    fprintf(tmpFile, "%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain);

    for (int j = 0; j < i; j++) {
        fprintf(tmpFile, "%d,%s,%.2f,%d\n", j+1, products[j].name, products[j].price, products[j].remain);
    }

    fclose(file);
    fclose(tmpFile);

    // Delete the old file and rename the temporaly file to the deleted file.
    remove("Data/MockUpProduct.csv");
    rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

    return 0;
}

int updateProduct(FILE *file, char* productName, Product newData) {
    /*
        Update provided product by given the name

        return 1 if the process is fail
        return 0 if the process success

    */

    char name[99];
    float price;
    int id, remain;
    int i = 0, flag = 0;

    Header test;

    Product products[999];

    FILE *tmpFile = fopen("Data/__MockUpProduct.csv", "w+");

    // Checking if unable to open file.
    if (tmpFile == NULL) {
        printf("Cant open file\n");
        return 1;
    }

    if (ferror(file)) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    // Get rid of the header
    fscanf(file, "%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain);

    // Loop until EOF
    while (!feof(file)) {
        fscanf(file, "%d,%[^,],%f,%d\n", &id, name, &price, &remain);
        // If the product in the current location is the same as the given product
        // If yes, just skip the part where putting the data in the array.

        if (!strcmp(name, productName)) {

            // Update the data
            products[i].id = id;
            products[i].name = copyString(productName, strlen(productName) + 1); // We add +1 because of the '\0'

            // Checking if the provided data are valid.
            products[i].price = ((int) newData.price) ? newData.price : price;
            products[i].remain = (newData.remain) ? newData.remain : remain;

            i++;
            flag = 1;
            continue;
        }

        products[i].id = id;
        products[i].name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'
        products[i].price = price;
        products[i].remain = remain;

        i++;
    }

    // Checking if the product exist.
    if (!flag) {
        printf("Product named '%s' doesn't exist.\n", productName);
        remove("Data/__MockUpProduct.csv");
        return 1;
    }

    // Write data in the temporaly file.
    fprintf(tmpFile, "%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain);

    for (int j = 0; j < i; j++) {
        fprintf(tmpFile, "%d,%s,%.2f,%d\n", j+1, products[j].name, products[j].price, products[j].remain);
    }

    fclose(file);
    fclose(tmpFile);

    // Delete the old file and rename the temporaly file to the deleted file.
    remove("Data/MockUpProduct.csv");
    rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

    return 0;
}
