#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "panels/panel.h"

void productAdminSeleted();

int main(void) {
   Setting setting;
   User user;

   char username[99];
   char password[99];

   int isLogin = 0;

   int logOrReg;

   printf("Welcome to the system\n");
   printf("Loading Setting....\n");

   if (checkSetting(&setting)) {
      printf("Setting is not set up yet.\n");
      printf("cls");
      printf("Please set up the setting first.\n");

      printf("Full stock: ");
      scanf(" %d", &setting.fullStock);
      printf("Threshold percent: ");
      scanf(" %d", &setting.ThresholdPercent);

      if (createSetting(setting)) {
         printf("Setting failed to be created.\n");
         return 1;
      }

      printf("Setting has been created.\n");
      delay(2);
      printf("\e[1;1H\e[2J");
   }
   else {
      delay(1);
      printf("Done.\n");
      delay(2);
      printf("\e[1;1H\e[2J");
   }

   delay(1);
   autoRestock(&setting);
   printf("\e[1;1H\e[2J");

   while (logOrReg != 1 && logOrReg != 2) {
      borderup();
      printf("   1. Login\n");
      printf("   2. Register\n");
      printf("   Choose: ");
      scanf(" %d", &logOrReg);
      if (logOrReg != 1 && logOrReg != 2) {
         printf("   Wrong input.\n");
         logOrReg = 0;
         delay(1);
         printf("\e[1;1H\e[2J");
         continue;
      }
      else {
         borderdown();
         delay(1);
         printf("\e[1;1H\e[2J");
         break;
      }
   }



   while (1) {
      if (isLogin == 0 && logOrReg == 1) {
         if (loginPanel("Login", username, password, &user)) {
            isLogin = 1;
            continue;
         }

         continue;
      }

      if (isLogin == 0 && logOrReg == 2) {
         if (registerPanel("Register", username, password, "customer")) {
            logOrReg = 1;
            continue;
         }

         continue;
      }

      if (strcmp(user.role, "admin") == 0) {
         printf("   Admin Panel\n");
         printf("   1. Product\n");
         printf("   2. User\n");
         printf("   3. Coupon\n");
         printf("   4. Setting\n");
         printf("   5. Logout\n");
         printf("   Choose: ");
         int choose;
         scanf(" %d", &choose);
         switch (choose) {
            case 1:
               delay(1);
               printf("\e[1;1H\e[2J");
               productAdminSeleted();
               break;
            case 2:
               // printUser();
               break;
            case 3:
               // printCoupon();
               break;
            case 4:
               // printSetting();
               break;
            case 5:
               // logout(&user);
               break;
            default:
               printf("   Wrong input.\n");
               break;
         }
      }
      else if (strcmp(user.role, "customer") == 0) {
         printf("   User Panel\n");
         printf("   1. Product\n");
         printf("   2. Coupon\n");
         printf("   3. Logout\n");
         printf("   Choose: ");
         int choose;
         scanf(" %d", &choose);
         switch (choose) {
            case 1:
               delay(1);
               printf("\e[1;1H\e[2J");
               printProduct();
               break;
            case 2:
               // printCoupon();
               break;
            case 3:
               // logout(&user);
               break;
            default:
               printf("   Wrong input.\n");
               break;
         }
      }
      else {
         printf("   Role not found.\n");
      }
   }
   return 0;
}

void productAdminSeleted() {
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
            printf("\e[1;1H\e[2J");
            createProductPanel();
            break;
         case 2:
            // updateProduct();
            break;
         case 3:
            // deleteProduct();
            break;
         case 4:
            // checkStock();
            break;
         case 5:
            // Restock();
            break;
         case 6:
            delay(1);
            printf("\e[1;1H\e[2J");
            return;
         default:
            printf("   Wrong input.\n");
            break;
      }
   }
}
