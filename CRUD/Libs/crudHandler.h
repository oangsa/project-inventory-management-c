#include <time.h>

typedef struct Product {
    int id;
    char *name;
    float price;
    int remain;
} Product;

typedef struct Setting {
    int ThresholdPercent;
    int fullStock;

    time_t lastCheck;
} Setting;

typedef struct Coupon {
    int id;
    char *code;
    int isUse;

    time_t expirationDate;
} Coupon;

// Product Handling
void printProduct();

void printProductFilterBy(char *filter);

int createProduct(Product product);

int updateProduct(char *productName, Product newData);

int deleteProduct(char *productName);

void checkStock(Setting *setting);

void Restock(Setting *setting);

void autoRestock(Setting *setting);


// User Handling


// Coupon Handling


// Setting Handling
int checkSetting(Setting *targetSetting);

int createSetting(Setting setting);

int updateSetting(Setting *setting);
