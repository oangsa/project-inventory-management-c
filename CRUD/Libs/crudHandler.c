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

void printProduct() {
    char name[99];
    float price;
    int id, remain;

    Header test;

    FILE *file = fopen("Data/MockUpProduct.csv", "a+");

    fscanf(file, "%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain);
    printf("-----------------------[Products]------------------------\n");
    printf("%-6s%-30s%-15s%-6s\n", test.no, test.name, test.price, test.remain);
    printf("%-6s%-30s%-15s%-6s\n", "--", "----", "-----", "------");

    while (!feof(file)) {
        fscanf(file, "%d,%[^,],%f,%d\n", &id, name, &price, &remain);

        printf("%-6d%-30s%-7.2f%-8s%-6d\n", id, name, price, "THB", remain);
    }

    fclose(file);
}

int createProduct(Product product) {
    /*
        Use to create a product

        return 1 if the process is fail
        return 0 if the process success
    */

    char name[99];
    float price;
    int id, remain;

    Header test;

    FILE *file = fopen("Data/MockUpProduct.csv", "a+");

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

    fclose(file);

    return 0;
}

int deleteProduct(char* productName) {
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

    FILE *file = fopen("Data/MockUpProduct.csv", "a+");

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

    // Delete the old file and rename the temporarily file to the deleted file.
    remove("Data/MockUpProduct.csv");
    rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

    return 0;
}

int updateProduct(char* productName, Product newData) {
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

    FILE *file = fopen("Data/MockUpProduct.csv", "a+");
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
        // If yes, just put the provided data into the array instead.
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

        fclose(file);
        fclose(tmpFile);
        return 1;
    }

    // Write data in the temporaly file.
    fprintf(tmpFile, "%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain);

    for (int j = 0; j < i; j++) {
        fprintf(tmpFile, "%d,%s,%.2f,%d\n", j+1, products[j].name, products[j].price, products[j].remain);
    }

    fclose(file);
    fclose(tmpFile);

    // Delete the old file and rename the temporarily file to the deleted file.
    remove("Data/MockUpProduct.csv");
    rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

    return 0;
}

void Restock(Setting *setting) {
    /*
        Restocking the product that have the remaining stock below
        the threshold

        return nothing
    */

    char name[99];
    float price;
    int id, remain;
    int i = 0, flag = 0;

    Header test;

    Product products[999];

    float expectLeastStock = (float) ((float) setting->ThresholdPercent / 100.00) * (float) setting->fullStock;

    FILE *file = fopen("Data/MockUpProduct.csv", "a+");
    FILE *tmpFile = fopen("Data/__MockUpProduct.csv", "w+");

    // Checking if unable to open file.
    if (tmpFile == NULL || file == NULL) {
        printf("Cant open file\n");
        return;
    }

    if (ferror(file)) {
        printf("%s\n", strerror(errno));
        return;
    }

    // Get rid of the header
    fscanf(file, "%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain);

    // Loop until EOF
    while (!feof(file)) {
        fscanf(file, "%d,%[^,],%f,%d\n", &id, name, &price, &remain);

        if (expectLeastStock > (float) remain) {
            remain = setting->fullStock;
        }

        products[i].id = id;
        products[i].name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'
        products[i].price = price;
        products[i].remain = remain;

        i++;
    }

    // Write data in the temporaly file.
    fprintf(tmpFile, "%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain);

    for (int j = 0; j < i; j++) {
        fprintf(tmpFile, "%d,%s,%.2f,%d\n", j+1, products[j].name, products[j].price, products[j].remain);
    }

    fclose(file);
    fclose(tmpFile);

    // Delete the old file and rename the temporarily file to the deleted file.
    remove("Data/MockUpProduct.csv");
    rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

    return;
}

void checkStock(Setting *setting) {

    char name[99];
    float price;
    int id, remain;
    int i = 0, flag = 0;
    char choice;

    Header test;
    Product products[999];

    FILE *file = fopen("Data/MockUpProduct.csv", "a+");

    float expectLeastStock = (float) ((float) setting->ThresholdPercent / 100.00) * (float) setting->fullStock;

    if (ferror(file)) {
        printf("%s\n", strerror(errno));
        return;
    }

    fscanf(file, "%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain);
    printf("-----------------------[Products]------------------------\n");
    printf("%-6s%-30s%-15s%-6s\n", test.no, test.name, test.price, test.remain);
    printf("%-6s%-30s%-15s%-6s\n", "--", "----", "-----", "------");

    while (!feof(file)) {
        fscanf(file, "%d,%[^,],%f,%d\n", &id, name, &price, &remain);

        if (expectLeastStock <= (float) remain) continue;

        printf("%-6d%-30s%-7.2f%-8s%-6d\n", id, name, price, "THB", remain);
        i++;
    }

    if (i != 0) {
        printf("\nThere is '%d' product(s) have the amount below threshold (%d%)\n", i, setting->ThresholdPercent);
        printf("Do you want to restock (y, n): ");
        scanf("%c", &choice);

        if (choice == 'y' || choice == 'Y') {
            fclose(file);
            Restock(setting);
        }
    }
    else {
        printf("There is no product that have the amount below threshold");
    }

    fclose(file);

}


// Setting Handler
int checkSetting(Setting *targetSetting) {
    /*
        This function use to check if the setting is already set

        return 1 if the process is fail
        return 0 if the process success

        and also if the setting exist, it should update the setting into the given variable.

    */
    Header test;
    Setting tmp;

    long time;

    FILE *file = fopen("Data/Setting.csv", "a+");


    if (ferror(file)) {
        printf("%s\n", strerror(errno));
        fclose(file);

        return 1;
    }

    fscanf(file, "%[^,],%[^,],%[^,\n]\n", test.no, test.name, test.price);

    fscanf(file, "%d,%d,%ld", &tmp.ThresholdPercent, &tmp.fullStock, &time);

    targetSetting->fullStock = tmp.fullStock;
    targetSetting->ThresholdPercent = tmp.ThresholdPercent;
    targetSetting->lastCheck = time;

    fclose(file);


    return 0;

}

int createSetting(Setting setting) {
    /*
        This function use to create the new setting.

        *CAUTION* : This function will overwrite the exist setting.

        return 1 if the process is fail
        return 0 if the process success

    */

    FILE *file = fopen("Data/Setting.csv", "a+");

    if (ferror(file)) {
        printf("%s\n", strerror(errno));
        fclose(file);
        return 1;
    }

    // Checking if the content in the file is empty
    fseek (file, 0, SEEK_END);
    long size = ftell(file);

    if (size) {
        printf("Setting file is already setup!\n");
        fclose(file);
        return 1;
    }

    FILE *tmpFile = fopen("Data/__Setting.csv", "w+");

    // Write the provided setting into the file
    fprintf(tmpFile, "%s,%s,%s\n", "Threshold", "Fullstock", "Last check");


    fprintf(tmpFile, "%d,%d,%ld\n", setting.ThresholdPercent, setting.fullStock, (long) setting.lastCheck);

    fclose(file);
    fclose(tmpFile);

    // Delete the old file and rename the temporarily file to the deleted file.
    remove("Data/Setting.csv");
    rename("Data/__Setting.csv", "Data/Setting.csv");

    return 0;


}
