#include <stdio.h>
#include <stdlib.h>
#include "panel.h"

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

void createProductPanel() {
   Product product;

   product.name = malloc(100 * sizeof(char));

   if (product.name == NULL) {
      printf("   Memory allocation failed.\n");
      delay(2);
      printf("\e[1;1H\e[2J");
      return;
   }

   borderup();
   printf("   Create Product\n");
   printf("   Name: ");
   scanf(" %s", product.name);
   printf("   Price: ");
   scanf(" %f", &product.price);
   printf("   Remain: ");
   scanf(" %d", &product.remain);

   if (createProduct(product) == 0) {
      printf("   Product has been created.\n");
      delay(2);
      printf("\e[1;1H\e[2J");
   }
   else {
      printf("   Product already exist.\n");
      delay(2);
      printf("\e[1;1H\e[2J");
   }

   free(product.name);
}
