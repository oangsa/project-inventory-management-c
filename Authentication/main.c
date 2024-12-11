#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "Libs/auth.h"
#include "Libs/utils.h"


int main() {
   int time;
   int attempt = 0;
   int choice;

   User user;

   char username[99];
   char password[99];
   char *text = "Enter Your Username and Password";

   system("cls");

   borderup();
   printf("  Welcome! Choose your choices\n  1:Login\n  2:Register\n  Please Enter your choice: ");
   scanf("%d", &choice);

   system("cls");

   if (choice != 1 && choice != 2) {
      printf("Invalid choice.");

      return 1;
   }

   // Register
   while (choice == 2 && attempt <= 3) {

      if (attempt == 3) {
         printf("Attempt limit exceed, please try again later.\n");
         printf("Auto-closed in (second)");

         for(int time = 5; time > 0; time--){
            delay(1);
            printf(" %d",time);
         }

         delay(1);
         system("cls");
         return 1;

      }
      else {
         borderup();
         printf("   %s       \n",text);
         printf("   Username : ");
         scanf(" %s", username);
         printf("   Password : ");
         scanf(" %s", password);

         if (!reg(username, password, "customer")) {
            printf("   Register successfully.\n");
            printf("   Welcome %s as a %s!\n", username, "customer");
            borderdown();
            choice = 0;
            break;
         }
         else {
            printf("   Username already taken.\n");
            printf("   Register failed.\n  (Please wait for 3 second to re-enter.)\n");
            delay(3);
            system("cls");
            attempt++;
         }
      }
   }

   // Login
   while(choice == 1 && attempt <= 3) {
      if (attempt == 3) {
         printf("Attempt limit exceed, please try again later.\n");
         printf("Auto-closed in (second)");

         for(int time = 5; time > 0; time--) {
            delay(1);
            printf(" %d",time);
         }

         delay(1);
         system("cls");
         return 0;
      }
      else {
         borderup();
         printf("   %s       \n",text);
         printf("   Username : ");
         scanf(" %s", username);

         printf("   Password : ");
         scanf(" %s", password);

         if (!login(username, password, &user)) {
            printf("   Login successfully.\n");
            printf("   Welcome %s as a %s!\n", user.username, user.role);
            borderdown();
            choice = 0;
            break;
         }
         else {
            printf("   Wrong username or password.\n  (Please wait for 3 second to re-enter.)\n");
            borderdown();
            delay(3);
            system("cls");
            attempt++;
         }
      }
   }

   return 0;
}
