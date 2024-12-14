#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>

#include "utils.h"
#include "../../Authentication/Libs/utils.h"

typedef struct UserHeader {
    char username[99];
    char password[99];
    char role[99];
} UserHeader;

typedef struct Header {
   char no[99];
   char name[99];
   char price[99];
   char remain[99];
   char sold[99];
} Header;


char* copyString(char *s, int size) {
   /*
      use to copy the string

      return given string for assign to the assignee.
   */
   char* s2;

   s2 = (char*) malloc((size * sizeof(char)) + 1);

   strcpy(s2, s);


   return (char*)s2;
}

int isTimePassed(time_t cmpTime, int day) {
    /*
        Use to check the given time if it pass the given day;

        return 1 if the given time is already passed
        return 0 if the given time haven't pass.

        Example:
            If the given timestamp is on 12/11/2024 00.00
            and the given day is 1.

            Suppose today is 13/11/2024 00.00
            and you call the function

            isTimePassed(oldTime, 1);

            the function should return 1;

            but if the given day is 2, the function would return 0;
    */

    // Change from day to second
    // Day * hrs in day * mins in hr * secs in min
    int expectPass = day * 24 * 60 * 60;
    time_t currentTime;

    time(&currentTime);

    // Compare the current time with the given time;
    double diffTime = difftime(currentTime, cmpTime);

    if ((double) expectPass >= diffTime) return 0;

    return 1;

}

int canPurchase(Product targetProduct) {
    /*
        Use to check if the product can be purchase or not.

        return 1 if the product can be purchase
        return 0 if the product can't be purchase
    */

   char* name;
   float price;
   int id, remain, sold;

   Header test;

   FILE *file = fopen("Data/MockUpProduct.csv", "r");

   if (file == NULL) {
      return 1;
   }

   // Get rid of the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   // Loop until EOF
   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      if (id == targetProduct.id) {
         if (remain >= targetProduct.remain) {
            fclose(file);
            return 1;
         }

         fclose(file);
         return 0;
      }
   }

   fclose(file);

   return 0;
}

ProductList *AppendOrEditProduct(ProductList *productList, Product targetProduct, int* counter, char *mode) {
   /*
      Use to append the product into the product list.

      return the new product list.
   */


      ProductList *head = productList;
      int flag = 0;


      // Loop until the end of the list
      if (!strcmp(mode, "userBuy")) {
         while (productList != NULL) {
            if (productList->product.id == targetProduct.id) {
               productList->product.remain += targetProduct.remain;
               flag = 1;
               break;
         }
         if (productList->next == NULL) break;
         productList = productList->next;
         }
      }

      if (!strcmp(mode, "loadSetting")) {
         while (productList != NULL) {
            if (!strcmp(productList->product.name, targetProduct.name)) {
                  productList->product.remain += targetProduct.remain;
                  flag = 1;
                  break;
            }
            if (productList->next == NULL) break;
            productList = productList->next;
         }
      }

      if (!flag) {

         ProductList *newNode = malloc(sizeof(ProductList));
         if (newNode == NULL) {
            perror("Failed to allocate memory");
         }

         // Increase the product count by 1 for every new product
         *counter++;

         newNode->product = targetProduct;
         newNode->next = NULL;

         if (productList != NULL) {
            productList->next = newNode;
         }
         else {
            head = newNode;
         }
      }

      return head;
}

int setup(User* user) {
   /*
      This function use to setup the data file if the file is not exist.

      return 1 if the process is fail
      return 0 if the process success

      also load the user data if the user exist.

   */
   int isUserExist = 1;

   FILE *file;

   char u[99];
   char p[99];
   char r[99] = "admin";

   printf("Checking the file....\n");
   delay(1);

   if (!isDirectoryExists("Data")) {
      printf("Data folder not found\n");
      delay(1);
      clearScreen();
      printf("Creating the data folder....\n");
      Log("Data folder not found, creating the data folder....");
      system("mkdir Data");
      delay(1);
      clearScreen();

      file = fopen("Data/MockUpProduct.csv", "a+");
      fprintf(file, "No,Name,Price,Remain,Sold\n");
      fclose(file);

      file = fopen("Data/users.csv", "a+");
      fprintf(file, "username,password,role\n");

      borderup();
      printf("   Create Admin: \n");
      printf("   Username: ");
      scanf(" %[^\n]s", u);
      printf("   Password: ");
      scanf(" %[^\n]s", p);
      borderdown();

      fprintf(file, "%s,%s,%s\n", u, p, r);

      Log("User '%s' has been created", u);

      if (isFileExists("Cache/__users.csv")) {
         FILE *cache = fopen("Cache/__users.csv", "w+");
         fprintf(cache, "username,password,role\n");
         fprintf(cache, "%s,%s,%s\n", u, p, r);
         fclose(cache);
      }


      fclose(file);

      file = fopen("Data/Setting.csv", "a+");
      fprintf(file, "Threshold,Fullstock,Last check\n");
      fclose(file);

      delay(1);
      clearScreen();

      isUserExist = 0;
   }
   else {
      if(!isFileExists("Data/MockUpProduct.csv")) {
         delay(1);
         printf("Product file not found....\n");
         delay(1);
         clearScreen();
         printf("Creating Product file....\n");
         Log("Product file not found, creating the product file....");

         file = fopen("Data/MockUpProduct.csv", "a+");
         fprintf(file, "No,Name,Price,Remain,Sold\n");
         fclose(file);
      }

      if (!isFileExists("Data/users.csv")) {
         delay(1);
         printf("User file not found....\n");
         delay(1);
         clearScreen();
         printf("Creating Product file....\n");
         Log("User file not found, creating the user file....");

         file = fopen("Data/users.csv", "a+");
         fprintf(file, "username,password,role\n");

         borderup();
         printf("   Create Admin: \n");
         printf("   Username: ");
         scanf(" %[^\n]s", u);
         printf("   Password: ");
         scanf(" %[^\n]s", p);
         borderdown();

         fprintf(file, "%s,%s,%s\n", u, p, r);

         Log("User '%s' has been created", u);

         if (isFileExists("Cache/__users.csv")) {
            FILE *cache = fopen("Cache/__users.csv", "w+");
            fprintf(cache, "username,password,role\n");
            fprintf(cache, "%s,%s,%s\n", u, p, r);
            fclose(cache);
         }

         fclose(file);
         delay(1);
         clearScreen();
      }

      if (!isFileExists("Data/Setting.csv")) {
         delay(1);
         printf("Setting file not found....\n");
         delay(1);
         clearScreen();
         printf("Creating Product file....\n");
         Log("Setting file not found, creating the setting file....");

         file = fopen("Data/Setting.csv", "a+");
         fprintf(file, "autoRestock,autoPurchase\n");
         fclose(file);
      }
   }

   if (!isDirectoryExists("Cache")) {
      printf("Cache folder not found\n");
      delay(1);
      clearScreen();
      printf("Creating the cache folder....\n");
      Log("Cache folder not found, creating the cache folder file....");
      system("mkdir Cache");
      delay(1);
      clearScreen();

      file = fopen("Cache/__users.csv", "a+");
      fprintf(file, "%s,%s,%s\n", u, p, r);
      fclose(file);

      isUserExist = 1;
   }
   else {
      if (!isFileExists("Cache/__MockUpProduct.csv")) {
         file = fopen("Cache/__MockUpProduct.csv", "a+");
         fclose(file);
      }

      if (!isFileExists("Cache/__users.csv")) {
         file = fopen("Cache/__users.csv", "a+");
         fprintf(file, "%s,%s,%s\n", u, p, r);
         fclose(file);

         isUserExist = 1;
      }
   }

   if (!isDirectoryExists("UserSetting")) {
      printf("UserSetting folder not found\n");
      delay(1);
      clearScreen();
      printf("Creating the UserSetting folder....\n");
      Log("UserSetting folder not found, creating the UserSetting folder file....");
      system("mkdir UserSetting");
      delay(1);
      clearScreen();
   }

   if (isUserExist == 1) {
      file = fopen("Cache/__users.csv", "a+");

      if (ferror(file) || file == NULL) {
         fclose(file);
         return 1;
      }

      fseek (file, 0, SEEK_END);
      long size = ftell(file);

      if (!size) {
         fclose(file);
         return 1;
      }

      fclose(file);

      file = fopen("Cache/__users.csv", "r");

      char Username[99];
      char Password[99];
      char Role[99];

      fscanf(file, "%[^,],%[^,],%[^,\n]\n", Username, Password, Role);

      user->username = copyString(Username, strlen(Username));
      user->password = copyString(Password, strlen(Password));
      user->role = copyString(Role, strlen(Role));

      fclose(file);

      Log("User '%s' login", user->username);

      return 0;
   }

   Log("No user found");

   return 1;
}

int isFileExists(char *path) {
    // Try to open file
    FILE *fptr = fopen(path, "r");

    // If file does not exists
    if (fptr == NULL) return 0;

    // File exists hence close file and return true.
    fclose(fptr);

    return 1;
}


int isDirectoryExists(char *path) {
   /**
    * Function to check whether a directory exists or not.
    * It returns 1 if given path is directory and  exists
    * otherwise returns 0.
    */
      struct stat stats;

      stat(path, &stats);

      // Check for file existence
      if (S_ISDIR(stats.st_mode)) return 1;

      return 0;
}
