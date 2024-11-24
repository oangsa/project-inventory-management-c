#include <time.h>

typedef struct Product {
    int id;
    char *name;
    char *description;
    float price;
    int remain;
} Product;

typedef struct User {
    int id;
    char *username;
    char *password;
    char *role;
} User;

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
void printProduct(FILE *file);

void printProductFilterBy(char *filter, FILE *file);

int createProduct(FILE *file, Product product);

int updateProduct(FILE *file, char* productName, Product newData);

int deleteProduct(FILE *file, char* productName);

void Restock(FILE *file, Setting *setting);


// User Handling


// Coupon Handling


// Setting Handling
int checkSetting(Setting *targetSetting, FILE *file);

int createSetting(FILE *file, Setting setting);
