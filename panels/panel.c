#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "panel.h"
#include "../LogSystem/LogHandler.h"
#include "../CouponSystem/Libs/coupon.h"

int setupPanel(Setting* setting, User* user, int* isUser) {
   /*
      * isUser = 0 -> Not login yet
      * isUser = 2 -> User
   */

   Log("System is starting...");
   clearScreen();
   if (!setup(user)) *isUser = 2;
   delay(1);
   printf("Done.\n");
   delay(1);

   clearScreen();
   printf("Loading Setting....\n");

   if (checkSetting(setting)) {
      printf("Setting is not set up yet.\n");
      clearScreen();
      printf("Please set up the setting first.\n");
      borderup();
      printf("   Full stock: ");
      scanf(" %d", &setting->fullStock);
      printf("   Threshold percent: ");
      scanf(" %d", &setting->ThresholdPercent);


      if (createSetting(*setting)) {
         printf("   Setting failed to be created.\n");
         borderup();
         delay(1);
         return 1;
      }

      printf("   Setting has been created.\n");
      Log("User '%s' setup a new setting.", user->username);
      borderdown();
      delay(1);
      checkSetting(setting);
      clearScreen();
      printf("Loading Setting....\n");
   }

   Log("Setting Loaded....");
   delay(1);
   printf("Done.\n");
   delay(1);
   clearScreen();
   printf("Caching product....\n");
   cacheProduct();
   delay(1);
   printf("Done.\n");
   Log("Product has been cached.");

   delay(1);
   Log("Work");
   Log("%s", user->role);
   if (!strcmp(user->role, "admin")) autoRestock(setting);
   if (!strcmp(user->role, "customer")) {
      clearScreen();
      printf("Attempting to auto purchase...\n");
      autoPurchase(user->username);
      delay(1);
   }
   clearScreen();
   delay(2);
   printf("Welcome to the system.\n");
   delay(2);
   clearScreen();

   return 0;
}

int askLoginOrRegister(int* logOrReg, int* isLogin) {
   while (*logOrReg != 1 && *logOrReg != 2 && *isLogin == 0 && *logOrReg != 3) {
      borderup();
      printf("   1. Login\n");
      printf("   2. Register\n");
      printf("   3. Exit\n");
      printf("   Choose: ");
      scanf(" %d", logOrReg);
      borderdown();

      if (*logOrReg != 1 && *logOrReg != 2 && *logOrReg != 3) {
         printf("   Wrong input.\n");
         *logOrReg = 0;
         delay(1);
         clearScreen();
         continue;
      }
      else {
         delay(1);
         clearScreen();
         break;
      }
   }

   return *logOrReg;
}

int registerPanel(char* text, char* username, char* password, char* role) {
   borderup();
   printf("   %s       \n",text);
   printf("   Username: ");
   scanf(" %s", username);

   printf("   Password: ");
   scanf(" %s", password);

   if (!reg(username, password, role)) {
      printf("   Register successfully.\n");
      printf("   Please Login again\n");
      Log("User '%s' has been registered as a %s.", username, role);
      borderdown();
      printf("Redirecting to the login page...\n");
      delay(3);
      clearScreen();

      return 1;
   }
   else {
      printf("   Username already exist.\n  (Please wait for 3 second to re-enter.)\n");
      borderdown();
      delay(3);
      clearScreen();

      return 0;
   }
}

int loginPanel(char* text, char* username, char* password, User* user) {
   borderup();
   printf("   %s       \n",text);
   printf("   Username: ");
   scanf(" %s", username);

   printf("   Password: ");
   scanf(" %s", password);

   if (!login(username, password, user)) {
      printf("   Login successfully.\n");
      printf("   Welcome %s as a %s!\n", user->username, user->role);
      Log("User '%s' has been logged in.", username);
      borderdown();
      printf("Redirecting to the main page...\n");
      delay(3);
      clearScreen();

      return 1;
   }
   else {
      printf("   Wrong username or password.\n  (Please wait for 3 second to re-enter.)\n");
      Log("User '%s' attempted to login.", username);
      borderdown();
      delay(3);
      clearScreen();

      return 0;
   }
}

void userPanel(User* user, char* reportDate) {
   int choose;

   char *select;

   select = malloc(100 * sizeof(char));

   while (1) {
      borderup();
      printf("   User Panel\n");
      printf("   1. Product\n");
      printf("   2. Coupon\n");
      printf("   3. Setting Auto Purchase\n");
      printf("   4. Logout\n");
      printf("   Choose: ");
      scanf(" %[^\n]", select);
      borderdown();

      choose = atoi(select);

      switch (choose) {
         case 1:
            Log("Product panel has been selected.");
            printf("Redirecting to the product panel...\n");
            delay(1);
            clearScreen();
            productUserSeleted(user);
            break;
         case 2:
            // printCoupon();
            break;
         case 3:
            Log("Setting auto purchase panel has been selected.");
            printf("Redirecting to the setting auto purchase panel...\n");
            delay(1);
            clearScreen();
            askAutoPurchasePanel(user);
            return;
         case 4:
            Log("User '%s' has been logged out.", user->username);
            logout(user);
            return;
         default:
            printf("   Wrong input.\n");
            break;
      }
   }
}

// User Controller
void productUserSeleted(User* user) {
   char *choose;
   int num;

   choose = malloc(100 * sizeof(char));

   while (1) {
      clearScreen();
      if (printProduct("customer")) {
         clearScreen();
         return;
      }

      printf("   Product\n");
      printf("   1. Buy\n");
      printf("   2. Back\n");
      printf("   Choose: ");
      scanf(" %[^\n]s", choose);

      num = atoi(choose);

      switch (num) {
         case 1:
            Log("Buy product triggered");
            clearScreen();
            // clearScreen();
            delay(1);
            if (printProduct("customer")) {
               clearScreen();
               return;
            }
            purcheaseProductPanel(user);
            break;
         case 2:
            delay(1);
            clearScreen();
            return;
         default:
            printf("   Wrong input.\n");
            delay(1);
            clearScreen();
            break;
      }
   }
}

void purcheaseProductPanel(User* user) {
   ProductList *productList;
   double total = 0;
   int counter = 0;

   productList = (ProductList*) malloc(sizeof(ProductList));

   if (productList == NULL) {
      Log("Memory allocation failed.");
      printf("   Memory allocation failed.\n");
      delay(2);
      clearScreen();
      return;
   }

   productList = NULL;

   char *choose;

   choose = malloc(100 * sizeof(char));

   if (choose == NULL) {
      Log("Memory allocation failed.");
      printf("   Memory allocation failed.\n");
      delay(2);
      clearScreen();
      return;
   }

   while(1) {
      Product product;
      char *id;

      id = malloc(100 * sizeof(char));

      if (id == NULL) {
         Log("Memory allocation failed.");
         printf("   Memory allocation failed.\n");
         delay(2);
         clearScreen();
         return;
      }

      clearScreen();
      printProduct("customer");
      printf("   Product\n");
      printf("   Id: ");
      scanf(" %[^\n]s", id);

      Log("User '%s' add product '%s' to the cart.", user->username, id);

      printf("   Amount: ");
      scanf(" %d", &product.remain);


      product.id = atoi(id);


      if (product.remain <= 0) {
         printf("   Amount must be greater than 0.\n");
         delay(2);
         clearScreen();
         continue;
      }

      if (!canPurchase(product, "normal")) {
         delay(2);
         clearScreen();
         printf("   Product is out of stock or the provided amount is greater than the stock..\n");
         delay(1);
         continue;
      }

      productList = AppendOrEditProduct(productList, product, &counter, "userBuy");
      Log("User '%s' add product '%d' to the cart(2).", user->username, product.id);

      printf("   Do you want to buy more? (Y/N): ");
      scanf(" %[^\n]s", choose);

      if (!strcmp(choose, "N") || !strcmp(choose, "n")) {
         ProductList *head = productList;

         while (1) {
            clearScreen();
            printf("   Total Product(s): %d\n", counter);
            printf("   Confirm purchase?\n");
            printf("   1. Yes\n");
            printf("   2. No\n");
            printf("   Choose: ");
            scanf(" %[^\n]s", choose);

            if (atoi(choose) == 1) {

               while (1) {
                  printf("   Would you like to use coupon? (Y/N): ");
                  scanf(" %[^\n]s", choose);

                  if (!strcmp(choose, "Y") || !strcmp(choose, "y")) {
                     if (useCouponPanel(user, productList, &total)) {
                        printf("   Coupon has been used.\n");
                        delay(2);
                        clearScreen();
                        Log("User '%s' purchase product(s) with total %.2f THB", user->username, total);
                        return;
                     }
                     else {
                        printf("   Coupon failed to be used.\n");
                        delay(2);
                        clearScreen();
                     }
                  }
                  else if (!strcmp(choose, "N") || !strcmp(choose, "n")) {
                     delay(1);
                     purchaseMultipleProduct(productList, &total);
                     delay(1);
                     clearScreen();
                     delay(1);
                     printf("   Total: %.2f THB\n", total);
                     delay(4);
                     clearScreen();
                     Log("User '%s' purchase product(s) with total %.2f THB", user->username, total);
                     return;
                  }
                  else {
                     printf("   Wrong input.\n");
                  }
               }
               return;
            }
            else if (atoi(choose) == 2) {
               delay(1);
               clearScreen();
               return;
            }
            else {
               printf("   Wrong input.\n");
            }
         }
      }
   }
}

int useCouponPanel(User* user, ProductList* productList, double* total) {
   char *choose;
   Coupon coupon;

   float discounted = 0;

   choose = malloc(100 * sizeof(char));

   if (choose == NULL) {
      printf("   Memory allocation failed.\n");
      delay(2);
      clearScreen();
      return 0;
   }

   purchaseMultipleProduct(productList, total);

   while (1) {
      clearScreen();
      displayCoupons("Data/Coupon.csv");
      borderup();
      printf("   Total: %.2f THB\n", *total);
      printf("   Coupon\n");
      printf("   Code: ");
      scanf(" %[^\n]s", coupon.code);

      coupon = getCouponByCode(coupon.code);

      if (coupon.id == -1) {
         printf("   Coupon not found.\n");
         borderdown();
         delay(2);
         clearScreen();
         continue;
      }

      if (isTimePassed(coupon.expirationDate, 0)) {
         printf("   Coupon has been expired.\n");
         borderdown();
         delay(2);
         clearScreen();
         continue;
      }

      if (!strcmp(coupon.type, "PERCENTAGE")) {
         discounted = *total * (coupon.discount / 100);
         printf("   You will get %.2f THB discount.\n", discounted);
      }
      else {
         if (coupon.minAmount > *total) {
            printf("    Minimum amount is %.2f THB.\n", coupon.minAmount);
            delay(2);
            clearScreen();
            continue;
         }
         discounted = coupon.discount;
         printf("    You will get %.2f THB discount.\n", discounted);
      }

      printf("   Do you want to use this coupon? (Y/N): ");
      scanf(" %[^\n]s", choose);

      if (!strcmp(choose, "Y") || !strcmp(choose, "y")) {
         Log("User '%s' use coupon '%s' with discount of %.2f THB", user->username, coupon.code, discounted);
         *total -= coupon.discount;
         free(choose);

         printf("   Total: %.2f THB\n", *total);
         delay(2);
         return 1;
      }
      else if (!strcmp(choose, "N") || !strcmp(choose, "n")) {
         free(choose);
         return 0;
      }
      else {
         printf("   Wrong input.\n");
         delay(2);
         clearScreen();
      }
   }

   free(choose);
}

void selectCouponAutoPurchasePanel(User *user) {
   Coupon coupon;

   char *choose;

   choose = malloc(100 * sizeof(char));

   if (!getTotalCoupons("Data/Coupon.csv")) {
      printf("No coupon available.\n");
      delay(2);
      clearScreen();
      return;
   }

   if (choose == NULL) {
      printf("   Memory allocation failed.\n");
      delay(2);
      clearScreen();
      return;
   }

   while (1) {
      clearScreen();
      displayCoupons("Data/Coupon.csv");
      borderup();
      printf("   Coupon\n");
      printf("   Code: ");
      scanf(" %[^\n]s", coupon.code);

      coupon = getCouponByCode(coupon.code);

      if (coupon.id == -1) {
         printf("   Coupon not found.\n");
         borderdown();
         delay(2);
         clearScreen();
         continue;
      }

      if (isTimePassed(coupon.expirationDate, 0)) {
         printf("   Coupon has been expired.\n");
         borderdown();
         delay(2);
         clearScreen();
         continue;
      }

      printf("   Do you want to use this coupon? (Y/N): ");
      scanf(" %[^\n]s", choose);

      if (!strcmp(choose, "Y") || !strcmp(choose, "y")) {

         if (writeCouponAutoPurchase(coupon, user->username)) {
            printf("   Coupon failed to be used.\n");
            delay(2);
            clearScreen();
            return;
         }

         printf("   Coupon has been used.\n");
         Log("User '%s' use coupon '%s' for auto purchase.", user->username, coupon.code);
         free(choose);
         return;
      }
      else if (!strcmp(choose, "N") || !strcmp(choose, "n")) {
         free(choose);
         return;
      }
      else {
         printf("   Wrong input.\n");
         delay(2);
         clearScreen();
      }
   }

}


void askAutoPurchasePanel(User* user) {
   char *choose;

   choose = malloc(100 * sizeof(char));

   if (choose == NULL) {
      printf("   Memory allocation failed.\n");
      delay(2);
      clearScreen();
      return;
   }

   while (1) {
      clearScreen();
      borderup();
      printf("   Auto Purchase System\n");
      printf("   1. Setup Auto Purchase\n");
      printf("   2. Setup Coupon For Auto Purchase\n");
      printf("   3. Back\n");
      printf("   Choose: ");
      scanf(" %[^\n]s", choose);

      if (!strcmp(choose, "1")) {
         clearScreen();
         delay(1);
         setUpAutoPurchasePanel(user);
      }
      else if (!strcmp(choose, "2")) {
         clearScreen();
         delay(1);
         selectCouponAutoPurchasePanel(user);
      }
      else {
         printf("   Wrong input.\n");
         delay(2);
         clearScreen();
      }
   }

   free(choose);
}


void setUpAutoPurchasePanel(User* user) {
   UserSetting userSetting;

   userSetting.product.name = malloc(100 * sizeof(char));

   while (1) {
      clearScreen();
      delay(1);
      printProduct("customer");
      printf("   Product (Auto Purchase)\n");
      printf("   Name: ");
      scanf(" %[^\n]", userSetting.product.name);
      printf("   Amount: ");
      scanf(" %d", &userSetting.product.remain);
      printf("   Day (0 = Sunday, 6 = Saturday): ");
      scanf(" %d", &userSetting.autoPurchaseDate);

      if (!createUserSetting(&userSetting, user->username)) {
         printf("   Auto purchase has been set up.\n");

         Log("User '%s' has been set up auto purchase.", user->username);
         clearScreen();
      }
      else {
         printf("   Auto purchase failed to be set up.\n");
         delay(2);
         clearScreen();
      }
   }
}


void adminPanel(User* user, Setting* setting) {
   int choose;

   while (1) {
      borderup();
      printf("   Admin Panel\n");
      printf("   1. Product\n");
      printf("   2. User\n");
      printf("   3. Coupon\n");
      printf("   4. Setting\n");
      printf("   5. Logout\n");
      printf("   Choose: ");
      scanf(" %d", &choose);
      borderdown();

      switch (choose) {
         case 1:
            delay(1);
            clearScreen();
            productAdminSeleted(setting);
            break;
         case 2:
            // printUser();
            break;
         case 3:
            delay(1);
            clearScreen();
            couponAdminSeleted();
            break;
         case 4:
            // printSetting();
            break;
         case 5:
            Log("Admin '%s' has been logged out.", user->username);
            logout(user);
            return;
         default:
            printf("   Wrong input.\n");
            break;
      }
   }
}


// Admin Controller
void couponAdminSeleted() {
   char *choose;

   choose = malloc(100 * sizeof(char));

   if (choose == NULL) {
      printf("   Memory allocation failed.\n");
      delay(2);
      clearScreen();
      return;
   }

   while (1) {
      clearScreen();
      displayCoupons("Data/Coupon.csv");
      borderup();
      printf("   Coupon Panel\n");
      printf("   1. Create Coupon\n");
      printf("   2. Delete Coupon\n");
      printf("   3. Back\n");
      printf("   Choose: ");
      scanf(" %[^\n]s", choose);
      borderdown();

      if (atoi(choose) == 1) {
         delay(1);
         clearScreen();
         addCoupon("Data/Coupon.csv");
      }
      else if (atoi(choose) == 2) {
         delay(1);
         clearScreen();
         displayCoupons("Data/Coupon.csv");
         deleteCoupon("Data/Coupon.csv");
      }
      else if (atoi(choose) == 3) {
         clearScreen();
         delay(1);
         return;
      }
      else {
         printf("   Wrong input.\n");
         delay(2);
         clearScreen();
      }
   }
}

// void createCouponPanel() {
//    Coupon coupon;

//    if (coupon.code == NULL) {
//       printf("   Memory allocation failed.\n");
//       delay(2);
//       clearScreen();
//       return;
//    }

//    borderup();
//    printf("   Create Coupon(Type E or e to exit.)\n");
//    printf("   Code: ");
//    scanf(" %s", coupon.code);

//    if (!strcmp(coupon.code, "E") || !strcmp(coupon.code, "e") || !strcmp(coupon.code, "E or e")) {
//       // free(coupon.code);
//       clearScreen();
//       return;
//    }

//    printf("   Discount: ");
//    scanf(" %f", &coupon.discount);
//    printf("   Expiration Date: ");
//    scanf(" %s", coupon.expirationDate);
//    printf("   Type: ");
//    scanf(" %s", coupon.type);
//    printf("   Min Amount: ");
//    scanf(" %f", &coupon.minAmount);

//    if (createCoupon(coupon) == 0) {
//       printf("   Coupon has been created.\n");
//       Log("Coupon '%s' has been created.", coupon.code);
//       delay(2);
//       clearScreen();
//    }
//    else {
//       printf("   Coupon already exist.\n");
//       delay(2);
//       clearScreen();
//    }
// }

void productAdminSeleted(Setting* setting) {
   int choose;

   while (1) {
      printf("   Product Panel\n");
      printf("   1. Create Product\n");
      printf("   2. Update Product\n");
      printf("   3. Delete Product\n");
      printf("   4. Check Stock\n");
      printf("   5. Restock\n");
      printf("   6. Back\n");
      printf("   Choose: ");

      scanf(" %d", &choose);

      switch (choose) {
         case 1:
            delay(1);
            clearScreen();
            createProductPanel();
            break;
         case 2:
            delay(1);
            clearScreen();
            updateProductPanel();
            break;
         case 3:
            delay(1);
            clearScreen();
            deleteProductPanel();
            break;
         case 4:
            delay(1);
            clearScreen();
            checkStock(setting);
            delay(1);
            clearScreen();
            break;
         case 5:
            // Restock();
            break;
         case 6:
            delay(1);
            clearScreen();
            return;
         default:
            printf("   Wrong input.\n");
            break;
      }
   }
}

void createProductPanel() {
   Product product;

   product.name = malloc(100 * sizeof(char));

   if (product.name == NULL) {
      printf("   Memory allocation failed.\n");
      delay(2);
      clearScreen();
      return;
   }

   borderup();
   printf("   Create Product(Type E or e to exit.)\n");
   printf("   Name: ");
   scanf(" %s", product.name);

   if (!strcmp(product.name, "E") || !strcmp(product.name, "e") || !strcmp(product.name, "E or e")) {
      free(product.name);
      clearScreen();
      return;
   }

   printf("   Price: ");
   scanf(" %f", &product.price);
   printf("   Remain: ");
   scanf(" %d", &product.remain);

   if (createProduct(product) == 0) {
      printf("   Product has been created.\n");
      Log("Product '%s' has been created.", product.name);
      delay(2);
      clearScreen();
   }
   else {
      printf("   Product already exist.\n");
      delay(2);
      clearScreen();
   }

   free(product.name);
}

void deleteProductPanel() {
   char *productId;

   productId = malloc(100 * sizeof(char));

   if (productId == NULL) {
      printf("   Memory allocation failed.\n");
      delay(2);
      clearScreen();
      return;
   }

   printProduct("admin");

   borderup();
   printf("   Delete Product(Type E or e to exit.)\n");
   printf("   Id: ");
   scanf(" %[^\n]s", productId);

   if (!strcmp(productId, "E") || !strcmp(productId, "e") || !strcmp(productId, "E or e")) {
      free(productId);
      clearScreen();
      return;
   }

   if (deleteProduct(atoi(productId)) == 0) {
      printf("   Product has been deleted.\n");
      Log("Product id '%s' has been deleted.", productId);
      borderdown();
      delay(2);
      clearScreen();
   }
   else {
      delay(2);
      clearScreen();
   }

   free(productId);
}

void updateProductPanel() {
   char *productId;
   Product product;
   Product targetProduct;

   productId = malloc(100 * sizeof(char));
   product.name = malloc(100 * sizeof(char));

   if (productId == NULL || product.name == NULL) {
      printf("   Memory allocation failed.\n");
      delay(2);
      clearScreen();
      return;
   }

   printProduct("admin");

   borderup();
   printf("   Update Product(Type E or e to exit.)\n");
   printf("   Id: ");
   scanf(" %[^\n]s", productId);

   if (!strcmp(productId, "E") || !strcmp(productId, "e") || !strcmp(productId, "E or e")) {
      free(productId);
      free(product.name);
      clearScreen();
      return;
   }

   getProduct(atoi(productId), &targetProduct);

   clearScreen();
   delay(1);

   printf("-----------------------------------------------------------------------------\n");
   printf("| %-2s  | %16s%-13s|%9s%-3s|%9s%-3s|%8s%-4s|\n", "Id", "Name"," ", "Price", " ", "Remain", " ", "Sold", " ");
   printf("|%s|%16s|%12s|%8s|%6s|\n", "-----", "------------------------------", "------------", "------------", "------------");
   printf("|  %-2d | %-28s | %-7.2f%s | %-4d%6s | %-4d%6s |\n", targetProduct.id, targetProduct.name, targetProduct.price, "THB", targetProduct.remain, "Items", targetProduct.sold, "Items");
   printf("-----------------------------------------------------------------------------\n");

   printf("   Name: ");
   scanf(" %[^\n]s", product.name);

   printf("   '%s' --> '%s'\n", targetProduct.name, product.name);

   printf("   Price: ");
   scanf(" %f", &product.price);

   printf("   '%.2f' --> '%.2f' (%.2f%s %s)\n", targetProduct.price, product.price, (targetProduct.price > product.price) ? (product.price / targetProduct.price) * 100 : ((product.price - targetProduct.price) / product.price) * 100, "%",(targetProduct.price > product.price) ? "Decrease" : "Increase");

   printf("   Remain: ");
   scanf(" %d", &product.remain);

   printf("   '%d' --> '%d' (%.2f%s %s)\n", targetProduct.remain, product.remain, ((float) targetProduct.remain > (float) product.remain) ? ((float) product.remain / (float) targetProduct.remain) * 100 : ((float) targetProduct.remain / (float) product.remain), "%",(targetProduct.remain > product.remain) ? "Decrease" : "Increase");

   if (updateProduct(atoi(productId), product) == 0) {
      printf("   Product has been updated.\n");
      Log("Product id '%s' has been updated.", productId);
      Log("'%s' has been updated to '%s'.", targetProduct.name, product.name);
      Log("'%f' has been updated to '%f'.", targetProduct.price, product.price);
      Log("'%d' has been updated to '%d'.", targetProduct.remain, product.remain);
      delay(1);
      clearScreen();
   }
   else {
      printf("   Product not found.\n");
      delay(1);
      clearScreen();
   }

   free(productId);
   free(product.name);
}
