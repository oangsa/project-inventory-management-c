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

// Product Handling
void printProduct(FILE *file);

void printProductFilterBy(char *filter, FILE *file);

int createProduct(FILE *file, Product product);

int updateProduct(FILE *file, char* productName);

int deleteProduct(FILE *file, char* productName);


// User Handling
