typedef struct product {
    char *name;
    char *description;
    float price;
    int remain;

} product;

void printProduct(FILE *file);

void printProductFilterBy(char *filter, FILE *file);
