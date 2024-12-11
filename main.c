#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Authentication/Libs/auth.h"
#include "Authentication/Libs/utils.h"
#include "CRUD/Libs/crudHandler.h"
#include "CRUD/Libs/utils.h"

int loginPanel(char* text, char* username, char* password, User* user);
int registerPanel(char* text, char* username, char* password, char* role);

int main(void) {
   Setting setting;
   User user;

   char username[99];
   char password[99];

   int isLogin = 0;

   int logOrReg;

   printf("Welcome to the system\n");
   printf("Checking...\n");

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
      printf("Done.\n");
      delay(2);
      printf("\e[1;1H\e[2J");
   }

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
               printProduct();
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
      borderdown();
      printf("Redirecting to the main page...\n");
      delay(3);
      printf("\e[1;1H\e[2J");

      return 1;
   }
   else {
      printf("   Wrong username or password.\n  (Please wait for 3 second to re-enter.)\n");
      borderdown();
      delay(3);
      printf("\e[1;1H\e[2J");

      return 0;
   }
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
      printf("   Please Login again");
      borderdown();
      printf("Redirecting to the login page...\n");
      delay(3);
      printf("\e[1;1H\e[2J");

      return 1;
   }
   else {
      printf("   Username already exist.\n  (Please wait for 3 second to re-enter.)\n");
      borderdown();
      delay(3);
      printf("\e[1;1H\e[2J");

      return 0;
   }
}
