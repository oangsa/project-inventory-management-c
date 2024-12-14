#include <time.h>

typedef struct Product {
    int id;
    char *name;
    float price;
    int remain;
    int sold;
} Product;

typedef struct ProductList {
    Product product;

    struct ProductList *next;
} ProductList;

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

typedef struct UserSetting {
   time_t lastCheck;

   // 0 -> Sun, 1 -> Mon, 2 -> Tue, 3 -> Wed, 4 -> Thu, 5 -> Fri, 6 -> Sat
   int autoPurchaseDate;

   // Use product.remain as a amount of product that the user want to purchase.
   Product product;

} UserSetting;

typedef struct UserSettingList {
   UserSetting setting;

   struct UserSettingList *next;
} UserSettingList;

// Product Handling
void cacheProduct();

void printProductFilterByName(char *name);

void printProductFilterByPrice(float min, float max);

int printProduct(char *mode);

int createProduct(Product product);

int updateProduct(int productId, Product newData);

int deleteProduct(int givenId);

int purchaseProduct(char *productName, int amount);

int purchaseMultipleProduct(ProductList *products, double *totalPrice);

int purchaseMultipleProductWithName(ProductList *products, double *totalPrice);

void getProduct(int productId, Product* product);

void checkStock(Setting *setting);

void Restock(Setting *setting);

void autoRestock(Setting *setting);

// User Handling
int createUserSetting(UserSetting *setting);

void LoadUserSettingList(ProductList *productList, int* ctr);

UserSettingList* AppendAndEditUserSettingList(UserSettingList *userSettingList, UserSetting userSetting);

void autoPurchase(UserSettingList *userSettingList, ProductList *productsList);

// Coupon Handling


// Setting Handling
int checkSetting(Setting *targetSetting);

int createSetting(Setting setting);

int updateSetting(Setting *setting);
