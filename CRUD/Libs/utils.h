#include <time.h>
#include "crudHandler.h"
#include "../../Authentication/Libs/utils.h"
#include "../../Authentication/Libs/auth.h"
#include "../../LogSystem/LogHandler.h"

char* copyString(char *s, int size);

int isTimePassed(time_t time, int day);

ProductList *AppendOrEditProduct(ProductList *productList, Product targetProduct, int* counter, char *mode);

int canPurchase(Product targetProduct, char* mode);

int setup(User *user);

int isDirectoryExists(char *path);

int isFileExists(char *path);

char* formatDateToString(int day, int month, int year);
