
#include "../CRUD/Libs/utils.h"
// #include "../CRUD/Libs/crudHandler.h"

int setupPanel(Setting* setting, User* user, int* isUser);

int loginPanel(char* text, char* username, char* password, User* user);

int askLoginOrRegister(int* logOrReg, int* isLogin);

int registerPanel(char* text, char* username, char* password, char* role);

void userPanel(User* user, char* reportDate);

// User Controller
void productUserSeleted(User* user);

void purcheaseProductPanel(User* user);

void adminPanel(User* user, Setting* setting);

void setUpAutoPurchasePanel(User* user);

int useCouponPanel(User* user, ProductList *productList, double *total);

// Admin Controller
void productAdminSeleted(Setting* setting);

void createProductPanel();

void deleteProductPanel();

void updateProductPanel();

void couponAdminSeleted();

void createCouponPanel();

void deleteCouponPanel();
