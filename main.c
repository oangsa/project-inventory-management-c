#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "panels/panel.h"

int main(void) {
   Setting setting;
   User user;

   char *reportName;

   reportName = (char*) malloc(99 * sizeof(char));

   if (reportName == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   time_t currentTime;
   time(&currentTime);

   struct tm *tm_local = localtime(&currentTime);

   char *dirDate = malloc(100 * sizeof(char));

   int day = tm_local->tm_mday;
   int month = tm_local->tm_mon + 1;
   int year = tm_local->tm_year + 1900;

   sprintf(reportName, "Report/%02d-%02d-%04d_report.csv", day, month, year);

   char username[99];
   char password[99];

   int isLogin = 0;

   int logOrReg;

   int loginAttempt = 0;

   // Setup the system
   if (setupPanel(&setting, &user, &isLogin)) {
      return 1;
   }


   while (1) {
      askLoginOrRegister(&logOrReg, &isLogin);

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
         if (loginAttempt == 3) {
            clearScreen();
            delay(1);
            printf("Too many login attempts. Exiting...\n");
            delay(1);
            break;
         }
         if (loginPanel("Login", username, password, &user, &loginAttempt)) {
            isLogin = 1;
            delay(1);
            Log("Work");
            Log("%s", user.role);
            if (!strcmp(user.role, "admin")) autoRestock(&setting);
            if (!strcmp(user.role, "customer")) {
               clearScreen();
               printf("Attempting to auto purchase...\n");
               autoPurchase(user.username);
               delay(1);
            }
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
         delay(1);
         clearScreen();
         delay(1);
         adminPanel(&user, &setting);
      }
      else if (strcmp(user.role, "customer") == 0) {
         if (isLogin == 2) {
            printf("Welcome back, %s!\n", user.username);
         }
         delay(1);
         clearScreen();
         delay(1);
         userPanel(&user, reportName);
      }
      else {
         printf("   Role not found.\n");
      }
   }
   return 0;
}
