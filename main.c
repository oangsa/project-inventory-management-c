#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "panels/panel.h"

int main(void) {
   Setting setting;
   User user;
   Product *purchasedProduct;

   UserSetting userSetting;

   char username[99];
   char password[99];

   int isLogin = 0;

   int logOrReg;

   // Setup the system
   if (setupPanel(&setting, &user, &isLogin)) {
      return 1;
   }

   askLoginOrRegister(&logOrReg, &isLogin);

   while (1) {
      if (user.username == NULL) {
         isLogin = 0;
         clearScreen();
         delay(1);
         askLoginOrRegister(&logOrReg, &isLogin);
      }

      if (logOrReg == 3) {
         printf("Goodbye!\n");
         break;
      }

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
         if (isLogin == 2) {
            printf("Welcome back, Admin %s!\n", user.username);
         }
         delay(2);
         clearScreen();
         delay(1);
         adminPanel(&user, &setting);
      }
      else if (strcmp(user.role, "customer") == 0) {
         if (isLogin == 2) {
            printf("Welcome back, %s!\n", user.username);
         }
         delay(2);
         clearScreen();
         delay(1);
         userPanel(&user);
      }
      else {
         printf("   Role not found.\n");
      }
   }
   return 0;
}
