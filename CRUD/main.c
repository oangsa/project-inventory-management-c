#include <stdio.h>
#include <stdlib.h>
#include "Libs\crudHandler.h"
#include "Libs\utils.h"

int main(void) {
    FILE *productFile;
    FILE *settingFile;
    Product newProduct;
    Setting newSetting;
    Setting *globalSetting = malloc(1 * sizeof(Setting));

    time_t curtime;

    time(&curtime);
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

    newSetting.fullStock = 100;
    newSetting.ThresholdPercent = 30;
    newSetting.lastCheck = curtime;

    productFile = fopen("Data/MockUpProduct.csv", "a+");
    settingFile = fopen("Data/Setting.csv", "a+");

    // Load setting
    checkSetting(globalSetting, settingFile);

    // Restocking
    Restock(productFile, globalSetting);

    // printf("%d\n", createSetting(settingFile, newSetting));

    // if(checkSetting(globalSetting, settingFile)) {
    //     printf("There's error during the setting checking...\n");
    //     return 1;
    // }

    // time_t test = (time_t) globalSetting->lastCheck;

    // if (isTimePassed(test, 1)) {
    //     printf("Yes\n");
    // }
    // else {
    //     printf("No\n");
    // }

    // printf("Difference is %.2f seconds\n", difftime(curtime, test));

    // printf("%s\n", test);

    // if (createProduct(productFile, newProduct)) {
    //     printf("There's error during the product creation...\n");

    //     return 1;
    // }

    // if (updateProduct(myFile, "Pad Krapao Moo", newProduct)) {
    //     printf("There's error during the product creation...\n");

    //     return 1;
    // }

    // printProduct(myFile);

    // printf("%d\n", createProduct(myFile, newProduct));

    fclose(productFile);
    fclose(settingFile);

    return 0;
}
