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

    Log("Time: %f", diffTime);
    Log("Expect: %d", expectPass);

    if ((double) expectPass >= diffTime) return 0;

    return 1;

}

int canPurchase(Product targetProduct, char* mode) {
    /*
        Use to check if the product can be purchase or not.

        return 1 if the product can be purchase
        return 0 if the product can't be purchase
    */

   char* name;
   float price;
   int id, remain, sold;

   name = (char*) malloc(999 * sizeof(char));

   Header test;

   FILE *file = fopen("Data/MockUpProduct.csv", "r");

   if (file == NULL) {
      return 1;
   }

   int Mode = 1;

   if (!strcmp(mode, "normal")) {
      Mode = 0;
   }

   // Get rid of the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   // Log("Header (CanPurchase): %s, %s, %s, %s, %s", test.no, test.name, test.price, test.remain, test.sold);

   // Log("Attempted Product (CanPurchase): %d, %s, %.2f, %d, %d", targetProduct.id, targetProduct.name, targetProduct.price, targetProduct.remain, targetProduct.sold);

   // Loop until EOF
   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      Log("Product (CanPurchase): %d, %s, %.2f, %d, %d", id, name, price, remain, sold);

      if (!Mode) {
         if (targetProduct.id == id) {
            if (remain >= targetProduct.remain) {
               fclose(file);
               free(name);
               return 1;
            }

            free(name);
            fclose(file);
            return 0;
         }
      }
      else {
         if (!strcmp(name, targetProduct.name)) {
            if (remain >= targetProduct.remain) {
               fclose(file);
               free(name);
               return 1;
            }

            printf("You are no longer be purchase this product as if I out of stock (%s).\n", name);
            fclose(file);
            free(name);
            return 0;
         }
      }
   }

   free(name);
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
      // Log("Product: %d", targetProduct.id);


      // Loop until the end of the list
      if (!strcmp(mode, "userBuy")) {
         while (productList != NULL) {
            Log("ProductList: %s", productList->product.name);
            if (productList->product.id == targetProduct.id) {
               if(!canPurchase(targetProduct, "normal")) {
                  printf("   Product is out of stock or the provided amount is greater than the stock..\n");
                  return head;
               }
               Log("HOW");
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
            // Log("ProductList: %s", productList->product.name);
            // Log("Target Product: %s", targetProduct.name);
            if (!strcmp(productList->product.name, targetProduct.name)) {

               // Log("Product List remain: %d", productList->product.remain);
               // Log("Target Product remain: %d", targetProduct.remain);

               productList->product.remain += targetProduct.remain;
               if(!canPurchase(targetProduct, "loadSetting")) {
                  printf("   Product is out of stock or the provided amount is greater than the stock..\n");
                  return head;
               }
               return head;
            }
            if (productList->next == NULL) break;
            productList = productList->next;
         }
      }

      Log("Flag: %d", flag);

      if (!flag) {

         ProductList *newNode = malloc(sizeof(ProductList));

         if (newNode == NULL) {
            perror("Failed to allocate memory");
         }

         // Increase the product count by 1 for every new product
         ++*counter;

         Log("Counter: %d", *counter);

         // newNode->product.name = malloc(999 * sizeof(char));

         Log("Product: %s", targetProduct.name);

         newNode->product.name = copyString(targetProduct.name, strlen(targetProduct.name) + 1);
         newNode->product.remain = targetProduct.remain;
         newNode->next = NULL;

         if (productList != NULL) {
            productList->next = newNode;
         }
         else {
            Log("Head: %s", newNode->product.name);
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
   int isUserExist = 0;

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

      file = fopen("Data/Coupon.csv", "a+");
      fprintf(file, "id,code,discount,expirationDate,type,minAmount\n");
      fclose(file);

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
      if (!isFileExists("Data/Coupon.csv")) {
         delay(1);
         printf("Coupon file not found....\n");
         delay(1);
         clearScreen();
         printf("Creating Coupon file....\n");
         Log("Coupon file not found, creating the coupon file....");

         file = fopen("Data/Coupon.csv", "a+");
         fprintf(file, "id,code,discount,expirationDate,type,minAmount\n");
         fclose(file);
      }

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
         fprintf(file, "Threshold,Fullstock,Last check\n");
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
      delay(1);
   }

   // I dont know why I have to put this here but if I dont put this here the program will crash
   printf("", isDirectoryExists("Report"));

   // delay(1);
   if (!isDirectoryExists("Report")) {
      printf("Report folder not found\n");
      delay(1);
      clearScreen();
      printf("Creating the Report folder....\n");
      Log("Report folder not found, creating the Report folder file....");
      system("mkdir Report");
      delay(1);
      clearScreen();

      time_t currentTime;
      time(&currentTime);

      struct tm *tm_local = localtime(&currentTime);

      char *dirDate = malloc(100 * sizeof(char));

      int day = tm_local->tm_mday;
      int month = tm_local->tm_mon + 1;
      int year = tm_local->tm_year + 1900;

      sprintf(dirDate, "Report/%02d-%02d-%04d_report.csv", day, month, year);

      if (!isFileExists(dirDate)) {
         file = fopen(dirDate, "a+");
         fprintf(file, "Earning without coupon,Earning with coupon,Total Earning,Total Product Sold\n");
         fprintf(file, "0.00,0.00,0.00,0\n");
         fclose(file);
      }
   }
   else {
      time_t currentTime;
      time(&currentTime);

      struct tm *tm_local = localtime(&currentTime);

      char *dirDate = malloc(100 * sizeof(char));

      int day = tm_local->tm_mday;
      int month = tm_local->tm_mon + 1;
      int year = tm_local->tm_year + 1900;

      sprintf(dirDate, "Report/%02d-%02d-%04d_report.csv", day, month, year);

      if (!isFileExists(dirDate)) {
         file = fopen(dirDate, "a+");
         fprintf(file, "Earning without coupon,Earning with coupon,Total Earning,Total Product Sold\n");
         fprintf(file, "0.00,0.00,0.00,0\n");
         fclose(file);
      }
   }

   if (isFileExists("Cache/__users.csv") && !isUserExist) {
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


      Log("TEST");

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
  struct stat buffer;

  if (stat (path, &buffer) == 0) return 1;

  return 0;
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


char* formatDateToString(int day, int month, int year) {
   char* result = (char*) malloc(20 * sizeof(char));

   if (result == NULL) {
      printf("Memory allocation failed!\n");
      return NULL;
   }

   sprintf(result, "%02d-%02d-%04d_report", day, month, year);

   return result;
}
