#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "utils.h"

// Just structure of the header to prevent the confusion.
typedef struct Header {
   char no[999];
   char name[999];
   char price[999];
   char remain[999];
   char sold[999];
} Header;

void cacheProduct() {
   /*
      Delete product by given the name

      return 1 if the process is fail
      return 0 if the process success

   */

   char* name;
   float price;
   int id, remain, sold;
   int i = 0, flag = 0;

   Product* products;

   name = (char*) malloc(9999 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return;
   }

   products = (Product*) malloc(999 * sizeof(Product));

   if (products == NULL) {
      free(name);
      printf("Memory allocation failed\n");
      return;
   }


   Header test;

   FILE *file = fopen("Data/MockUpProduct.csv", "r");

   FILE *tmpFile = fopen("Cache/__MockUpProduct.csv", "w+");
   // Checking if unable to open file.
   if (tmpFile == NULL || file == NULL) {
      printf("Cant open file\n");
      printf("%s\n", strerror(errno));
      return;
   }

   if (ferror(file) || ferror(tmpFile)) {
      printf("%s\n", strerror(errno));
      return;
   }

   // Get rid of the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]\n", test.no, test.name, test.price, test.remain, test.sold);
   // Loop until EOF
   while (!feof(file)) {
      fscanf(file, "%d,%[^,\n],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      // If the product in the current location is the same as the given product
      // If yes, just skip the part where putting the data in the array.

      products[i].id = id;
      products[i].name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'
      products[i].price = price;
      products[i].remain = remain;
      products[i].sold = sold;

      i++;
   }

   // Write data in the cache file.
   fprintf(tmpFile, "%s,%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain, test.sold);

   for (int j = 0; j < i; j++) {
      fprintf(tmpFile, "%d,%s,%.2f,%d,%d\n", j+1, products[j].name, products[j].price, products[j].remain, products[j].sold);
   }

   free(name);
   free(products);
   fclose(file);
   fclose(tmpFile);

   return;
}

int printProduct(char *mode) {
   char* name;
   float price;
   int id = 0, remain, sold;

   int isNoProduct = 1;

   name = (char*) malloc(99 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   Header test;

   FILE *file = fopen("Data/MockUpProduct.csv", "r");


   if (file == NULL) {
      printf("%s\n", strerror(errno));
      return 1;
   }

   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

   while (!feof(file)) {

      if (isNoProduct) {
         printf("+--------------------------------[Products]---------------------------------+\n");
         printf("| %-2s  | %16s%-13s|%9s%-3s|%9s%-3s|%8s%-4s|\n", "Id", test.name," ", test.price, " ", test.remain, " ", test.sold, " ");
         printf("|%s|%16s|%12s|%8s|%6s|\n", "-----", "------------------------------", "------------", "------------", "------------");
         isNoProduct = 0;
      }
      printf("|  %-2d | %-28s | %-7.2f%s | %-4d%6s | %-4d%6s |\n", id, name, price, "THB", remain, "Items", sold, "Items");

      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      if (remain == 0 && strcmp(mode, "customer") == 0) continue;

   }

   if (id && isNoProduct) {
      printf("+--------------------------------[Products]---------------------------------+\n");
      printf("| %-2s  | %16s%-13s|%9s%-3s|%9s%-3s|%8s%-4s|\n", "Id", test.name," ", test.price, " ", test.remain, " ", test.sold, " ");
      printf("|%s|%16s|%12s|%8s|%6s|\n", "-----", "------------------------------", "------------", "------------", "------------");
      isNoProduct = 0;
   }

   if (!isNoProduct) {
      printf("|  %-2d | %-28s | %-7.2f%s | %-4d%6s | %-4d%6s |\n", id, name, price, "THB", remain, "Items", sold, "Items");
      printf("+---------------------------------------------------------------------------+\n");
   }

   if (isNoProduct) {
      printf("No product available.\n");
      delay(1);
      return 1;
   }


   free(name);
   fclose(file);

   return 0;
}

int printProductFilterByName(char *name, char *mode) {
   char* Name;
   float price;
   int id, remain, sold;

   int isNoProduct = 1;

   Name = (char*) malloc(99 * sizeof(char));

   if (Name == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   Header test;

   FILE *file = fopen("Data/MockUpProduct.csv", "r");


   if (file == NULL) {
      printf("%s\n", strerror(errno));
      return 1;
   }

   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, Name, &price, &remain, &sold);

   while (!feof(file)) {
      // Skip the product if the name is not the same as the given name and skip the product if the remain is 0 and the mode is customer.
      Log("Name: %s, name: %s", Name, name);

      if (!strcmp(Name, name)) {
         if (isNoProduct) {
            printf("+--------------------------------[Products]---------------------------------+\n");
            printf("| %-2s  | %16s%-13s|%9s%-3s|%9s%-3s|%8s%-4s|\n", "Id", test.name," ", test.price, " ", test.remain, " ", test.sold, " ");
            printf("|%s|%16s|%12s|%8s|%6s|\n", "-----", "------------------------------", "------------", "------------", "------------");
            isNoProduct = 0;
         }
         printf("|  %-2d | %-28s | %-7.2f%s | %-4d%6s | %-4d%6s |\n", id, name, price, "THB", remain, "Items", sold, "Items");
      }

      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, Name, &price, &remain, &sold);

      if (remain == 0 && strcmp(mode, "customer") == 0) continue;

   }

   if (!isNoProduct || strcmp(Name, name) == 0) {
      if (isNoProduct) {
         printf("+--------------------------------[Products]---------------------------------+\n");
         printf("| %-2s  | %16s%-13s|%9s%-3s|%9s%-3s|%8s%-4s|\n", "Id", test.name," ", test.price, " ", test.remain, " ", test.sold, " ");
         printf("|%s|%16s|%12s|%8s|%6s|\n", "-----", "------------------------------", "------------", "------------", "------------");
         printf("|  %-2d | %-28s | %-7.2f%s | %-4d%6s | %-4d%6s |\n", id, name, price, "THB", remain, "Items", sold, "Items");
      }
      isNoProduct = 0;
      printf("+---------------------------------------------------------------------------+\n");
   }

   if (isNoProduct) {
      printf("No product available.\n");
      delay(1);
      return 1;
   }


   free(name);
   fclose(file);

   return 0;
}

int printProductFilterByPrice(float min, float max, char *mode) {
   char* Name;
   float price;
   int id, remain, sold;

   int isNoProduct = 1;

   Name = (char*) malloc(99 * sizeof(char));

   if (Name == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   Header test;

   FILE *file = fopen("Data/MockUpProduct.csv", "r");


   if (file == NULL) {
      printf("%s\n", strerror(errno));
      return 1;
   }

   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, Name, &price, &remain, &sold);

   while (!feof(file)) {
      // Skip the product if the name is not the same as the given name and skip the product if the remain is 0 and the mode is customer.
      if (price <= max && price >= min) {
         if (isNoProduct) {
            printf("+--------------------------------[Products]---------------------------------+\n");
            printf("| %-2s  | %16s%-13s|%9s%-3s|%9s%-3s|%8s%-4s|\n", "Id", test.name," ", test.price, " ", test.remain, " ", test.sold, " ");
            printf("|%s|%16s|%12s|%8s|%6s|\n", "-----", "------------------------------", "------------", "------------", "------------");
            isNoProduct = 0;
         }
         printf("|  %-2d | %-28s | %-7.2f%s | %-4d%6s | %-4d%6s |\n", id, Name, price, "THB", remain, "Items", sold, "Items");
      }

      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, Name, &price, &remain, &sold);

      if (remain == 0 && strcmp(mode, "customer") == 0) continue;

   }

   if (!isNoProduct || (price <= max && price >= min)) {
      if (isNoProduct) {
         printf("+--------------------------------[Products]---------------------------------+\n");
         printf("| %-2s  | %16s%-13s|%9s%-3s|%9s%-3s|%8s%-4s|\n", "Id", test.name," ", test.price, " ", test.remain, " ", test.sold, " ");
         printf("|%s|%16s|%12s|%8s|%6s|\n", "-----", "------------------------------", "------------", "------------", "------------");
         printf("|  %-2d | %-28s | %-7.2f%s | %-4d%6s | %-4d%6s |\n", id, Name, price, "THB", remain, "Items", sold, "Items");
         isNoProduct = 0;
      }
      printf("+---------------------------------------------------------------------------+\n");
   }

   if (isNoProduct) {
      printf("No product available.\n");
      delay(1);
      return 1;
   }


   free(Name);
   fclose(file);

   return 0;
}

int createProduct(Product product) {
   /*
      Use to create a product

      return 1 if the process is fail
      return 0 if the process success
   */

   char* name;
   float price;
   int id = 0, remain, sold;

   name = (char*) malloc(99 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   Header test;

   FILE *file = fopen("Data/MockUpProduct.csv", "a+");

   // Error handling
   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return 1;
   }

   // Handling edge cases where user havent provide the product information.
   if (!product.name || !product.price || !product.remain) {
      printf("No enough information\n");

      return 1;
   }

   // Get rid the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   // Loop checking if the provided product is already exist.
   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      if (!strcmp(name, product.name)) {
         free(name);
         fclose(file);
         return 1;
      }
   }

   fprintf(file, "%d,%s,%.2f,%d,%d\n", id + 1, product.name, product.price, product.remain, 0);

   free(name);
   free(file);
   fclose(file);
   fclose(file);

   return 0;
}

int deleteProduct(int givenId) {
   /*
      Delete product by given the name

      return 1 if the process is fail
      return 0 if the process success

   */

   char* name;
   float price;
   int id, remain, sold;
   int i = 0, flag = 0;

   Product* products;

   name = (char*) malloc(99 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   products = (Product*) malloc(999 * sizeof(Product));

   if (products == NULL) {
      free(name);
      printf("Memory allocation failed\n");
      return 1;
   }


   Header test;

   FILE *file = fopen("Data/MockUpProduct.csv", "a+");

   FILE *tmpFile = fopen("Data/__MockUpProduct.csv", "w+");

   // Checking if unable to open file.
   if (tmpFile == NULL) {
      printf("Cant open file\n");
      return 1;
   }

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      return 1;
   }

   // Get rid of the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   // Loop until EOF
   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      // If the product in the current location is the same as the given product
      // If yes, just skip the part where putting the data in the array.
      if (givenId == id) {
         flag = 1;
         continue;
      }

      products[i].id = id;
      products[i].name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'
      products[i].price = price;
      products[i].remain = remain;
      products[i].sold = sold;

      i++;
   }

   // Checking if the product exist.
   if (!flag) {
      printf("   Product id '%d' doesn't exist.\n", givenId);
      remove("Data/__MockUpProduct.csv");
      free(name);
      free(products);
      fclose(file);
      fclose(tmpFile);
      return 1;
   }

   // Write data in the temporaly file.
   fprintf(tmpFile, "%s,%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain, test.sold);

   for (int j = 0; j < i; j++) {
      fprintf(tmpFile, "%d,%s,%.2f,%d,%d\n", j+1, products[j].name, products[j].price, products[j].remain, products[j].sold);
   }

   free(name);
   free(products);
   fclose(file);
   fclose(tmpFile);

   // Delete the old file and rename the temporarily file to the deleted file.
   remove("Data/MockUpProduct.csv");
   rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

   return 0;
}

int updateProduct(int productId, Product newData) {
   /*
      Update provided product by given the id

      return 1 if the process is fail
      return 0 if the process success

   */

   char* name;
   float price;
   int id, remain, sold;
   int i = 0, flag = 0;

   Header test;

   Product* products;

   name = (char*) malloc(99 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   products = (Product*) malloc(999 * sizeof(Product));

   if (products == NULL) {
      free(name);
      printf("Memory allocation failed\n");
      return 1;
   }

   FILE *file = fopen("Data/MockUpProduct.csv", "a+");
   FILE *tmpFile = fopen("Data/__MockUpProduct.csv", "w+");

   // Checking if unable to open file.
   if (tmpFile == NULL) {
      printf("Cant open file\n");
      return 1;
   }

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      return 1;
   }

   // Get rid of the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   // Loop until EOF
   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      // If the product in the current location is the same as the given product
      // If yes, just put the provided data into the array instead.

      if (!strcmp(name, newData.name) && productId != id) {
         printf("   Product name '%s' already exist.\n", newData.name);
         remove("Data/__MockUpProduct.csv");

         free(name);
         free(products);
         fclose(file);
         fclose(tmpFile);
         return 1;
      }

      if (productId == id) {

         // Update the data
         products[i].id = id;
         products[i].name = copyString(newData.name, strlen(newData.name) + 1); // We add +1 because of the '\0'
         products[i].sold = sold;

         // Checking if the provided data are valid.
         products[i].price = ((int) newData.price) ? newData.price : price;
         products[i].remain = (newData.remain) ? newData.remain : remain;

         i++;
         flag = 1;
         continue;
      }

      products[i].id = id;
      products[i].name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'
      products[i].price = price;
      products[i].remain = remain;
      products[i].sold = sold;

      i++;
   }

   // Checking if the product exist.
   if (!flag) {
      printf("   Product id '%d' doesn't exist.\n", productId);
      remove("Data/__MockUpProduct.csv");

      free(name);
      free(products);
      fclose(file);
      fclose(tmpFile);
      return 1;
   }

   // Write data in the temporaly file.
   fprintf(tmpFile, "%s,%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain, test.sold);

   for (int j = 0; j < i; j++) {
      fprintf(tmpFile, "%d,%s,%.2f,%d,%d\n", j+1, products[j].name, products[j].price, products[j].remain, products[j].sold);
   }

   free(name);
   free(products);
   fclose(file);
   fclose(tmpFile);

   // Delete the old file and rename the temporarily file to the deleted file.
   remove("Data/MockUpProduct.csv");
   rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

   fclose(file);
   fclose(tmpFile);

   return 0;
}

int purchaseProduct(char* productName, int amount) {
   /*
      Purchase provided product by given the name

      return 1 if the process is fail
      return 0 if the process success

   */

   char* name;
   float price;
   int id, remain, sold;
   int i = 0, flag = 0;

   Header test;

   Product* products;

   name = (char*) malloc(99 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   products = (Product*) malloc(999 * sizeof(Product));

   if (products == NULL) {
      free(name);
      printf("Memory allocation failed\n");
      return 1;
   }

   FILE *file = fopen("Data/MockUpProduct.csv", "a+");
   FILE *tmpFile = fopen("Data/__MockUpProduct.csv", "w+");

   // Checking if unable to open file.
   if (tmpFile == NULL) {
      printf("Cant open file\n");
      return 1;
   }

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      return 1;
   }

   // Get rid of the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   // Loop until EOF
   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      // If the product in the current location is the same as the given product
      // If yes, just put the provided data into the array instead.
      if (!strcmp(name, productName)) {

         if (amount > remain) {
            printf("    Not enough stock\n");

            free(name);
            free(products);
            fclose(file);
            fclose(tmpFile);

            remove("Data/__MockUpProduct.csv");
            return 1;
         }

         // Update the data
         products[i].id = id;
         products[i].name = copyString(productName, strlen(productName) + 1); // We add +1 because of the '\0'

         // Checking if the provided data are valid.
         products[i].price = price;
         products[i].remain = remain - amount;
         products[i].sold = sold + amount;

         i++;
         flag = 1;
         continue;
      }

      products[i].id = id;
      products[i].name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'
      products[i].price = price;
      products[i].remain = remain;
      products[i].sold = remain;

      i++;
   }

   // Checking if the product exist.
   if (!flag) {
      printf("    Product named '%s' doesn't exist.\n", productName);
      remove("Data/__MockUpProduct.csv");

      free(name);
      free(products);
      fclose(file);
      fclose(tmpFile);
      return 1;
   }

   // Write data in the temporaly file.
   fprintf(tmpFile, "%s,%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain, test.sold);

   for (int j = 0; j < i; j++) {
      fprintf(tmpFile, "%d,%s,%.2f,%d,%d\n", j+1, products[j].name, products[j].price, products[j].remain, products[j].sold);
   }

   free(name);
   free(products);
   fclose(file);
   fclose(tmpFile);

   // Delete the old file and rename the temporarily file to the deleted file.
   remove("Data/MockUpProduct.csv");
   rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

   return 0;
}


int purchaseMultipleProduct(ProductList *products, double *totalPrice) {
   /*
      Purchase provided product by given the name

      return 1 if the process is fail
      return 0 if the process success

   */

   // Use remain form the ProductList as the amount of the product that the user want to purchase.
   ProductList* temp = products;

   char* name;
   float price;
   int id, remain, sold;
   int i = 0, flag = 0;

   Header test;

   ProductList* writeProducts;

   name = (char*) malloc(99 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   writeProducts = (ProductList*) malloc(999 * sizeof(ProductList));

   if (writeProducts == NULL) {
      free(name);
      printf("Memory allocation failed\n");
      return 1;
   }

   FILE *file = fopen("Data/MockUpProduct.csv", "r");

   FILE *tmpFile = fopen("Data/__MockUpProduct.csv", "w+");

   // Checking if unable to open file.

   if (tmpFile == NULL || file == NULL) {
      printf("Cant open file\n");
      return 1;
   }

   if (ferror(file) || ferror(tmpFile)) {
      printf("%s\n", strerror(errno));
      return 1;
   }


   // Get rid of the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      // Log("Product '%d' has been purchased.", id);

      temp = products;
      flag = 0;

      while (temp != NULL) {

         if (temp->product.id == id) {

            writeProducts[i].product.id = id;
            writeProducts[i].product.name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'

            // Checking if the provided data are valid.
            writeProducts[i].product.price = price;
            writeProducts[i].product.remain = remain - temp->product.remain;
            writeProducts[i].product.sold = sold + temp->product.remain;

            *totalPrice += ((double) temp->product.remain * price);

            updateDailySummary(0, temp->product.remain, "purchase");

            i++;

            temp = temp->next;

            flag = 1;
            break;
         }

         temp = temp->next;
      }

      if (!flag) {
         // Update the data
         writeProducts[i].product.id = id;
         writeProducts[i].product.name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'

         // Checking if the provided data are valid.
         writeProducts[i].product.price = price;
         writeProducts[i].product.remain = remain;
         writeProducts[i].product.sold = sold;

         i++;
      }
   }

   //Write data in the temporaly file.
   fprintf(tmpFile, "%s,%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain, test.sold);

   for (int j = 0; j < i; j++) {
      fprintf(tmpFile, "%d,%s,%.2f,%d,%d\n", j+1, writeProducts[j].product.name, writeProducts[j].product.price, writeProducts[j].product.remain, writeProducts[j].product.sold);
   }

   free(name);
   free(writeProducts);
   fclose(file);
   fclose(tmpFile);

   // Delete the old file and rename the temporarily file to the deleted file.
   remove("Data/MockUpProduct.csv");
   rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

   return 0;
}

int purchaseMultipleProductWithName(ProductList *products, double *totalPrice) {
   /*
      Purchase provided product by given the name

      return 1 if the process is fail
      return 0 if the process success

   */

   // Use remain form the ProductList as the amount of the product that the user want to purchase.
   ProductList* temp = products;

   char* name;
   float price;
   int id, remain, sold;
   int i = 0, flag = 0;

   Header test;

   ProductList* writeProducts;

   name = (char*) malloc(99 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return 1;
   }

   writeProducts = (ProductList*) malloc(999 * sizeof(ProductList));

   if (writeProducts == NULL) {
      free(name);
      printf("Memory allocation failed\n");
      return 1;
   }

   FILE *file = fopen("Data/MockUpProduct.csv", "r");

   FILE *tmpFile = fopen("Data/__MockUpProduct.csv", "w+");

   // Checking if unable to open file.

   if (tmpFile == NULL || file == NULL) {
      printf("Cant open file\n");
      return 1;
   }

   if (ferror(file) || ferror(tmpFile)) {
      printf("%s\n", strerror(errno));
      return 1;
   }

   // Get rid of the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      temp = products;
      flag = 0;

      while (temp != NULL) {

         Log("(tmp) Product '%s' has been purchased.", temp->product.name);


         if (!strcmp(temp->product.name, name)) {

            writeProducts[i].product.id = id;
            writeProducts[i].product.name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'

            // Checking if the provided data are valid.
            writeProducts[i].product.price = price;
            writeProducts[i].product.remain = remain - temp->product.remain;
            writeProducts[i].product.sold = sold + temp->product.remain;

            *totalPrice += ((double) temp->product.remain * price);

            i++;

            temp = temp->next;

            flag = 1;
            break;
         }

         temp = temp->next;
      }

      if (!flag) {
         // Update the data
         writeProducts[i].product.id = id;
         writeProducts[i].product.name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'

         // Checking if the provided data are valid.
         writeProducts[i].product.price = price;
         writeProducts[i].product.remain = remain;
         writeProducts[i].product.sold = sold;

         i++;
      }
   }

   //Write data in the temporaly file.
   fprintf(tmpFile, "%s,%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain, test.sold);

   for (int j = 0; j < i; j++) {
      fprintf(tmpFile, "%d,%s,%.2f,%d,%d\n", j+1, writeProducts[j].product.name, writeProducts[j].product.price, writeProducts[j].product.remain, writeProducts[j].product.sold);
   }

   free(name);
   free(writeProducts);
   fclose(file);
   fclose(tmpFile);

   // Delete the old file and rename the temporarily file to the deleted file.
   remove("Data/MockUpProduct.csv");
   rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

   return 0;
}


void getProduct(int productId, Product* product) {
   /*
      Get product by given the id

      return nothing
   */

   char* name;
   float price;
   int id, remain, sold;
   int i = 0, flag = 0;

   Header test;

   Product* products;

   name = (char*) malloc(99 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return;
   }

   products = (Product*) malloc(999 * sizeof(Product));

   if (products == NULL) {
      free(name);
      printf("Memory allocation failed\n");
      return;
   }

   FILE *file = fopen("Data/MockUpProduct.csv", "r");

   // Checking if unable to open file.
   if (file == NULL) {
      printf("Cant open file\n");
      return;
   }

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      return;
   }

   // Get rid of the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   // Loop until EOF
   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      if (productId == id) {
         product->id = id;
         product->name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'
         product->price = price;
         product->remain = remain;
         product->sold = sold;

         printf("Product id: %d\n", product->id);
         printf("Product name: %s\n", product->name);
         printf("Product price: %.2f\n", product->price);
         printf("Product remaining: %d\n", product->remain);
         printf("Product sold: %d\n", product->sold);

         flag = 1;
         break;
      }
   }

   if (!flag) {
      printf("Product id '%d' doesn't exist.\n", productId);
   }

   free(name);
   free(products);
   fclose(file);
   fclose(file);

   return;
}

void Restock(Setting *setting, char* mode) {
   /*
      Restocking the product that have the remaining stock below
      the threshold

      return nothing
   */

   char *name;
   float price;
   int id, remain, sold;
   int i = 0, flag = 0;

   Header test;

   Product *products;

   name = (char*) malloc(99 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return;
   }

   products = (Product*) malloc(999 * sizeof(Product));

   if (products == NULL) {
      free(name);
      printf("Memory allocation failed\n");
      return;
   }

   float expectLeastStock = (float) ((float) setting->ThresholdPercent / 100.00) * (float) setting->fullStock;

   FILE *file = fopen("Data/MockUpProduct.csv", "a+");
   FILE *tmpFile = fopen("Data/__MockUpProduct.csv", "w+");

   // Checking if unable to open file.
   if (tmpFile == NULL || file == NULL) {
      printf("Cant open file\n");
      return;
   }

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      return;
   }

   // Get rid of the header
   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   // Loop until EOF
   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      if (expectLeastStock > (float) remain || !strcmp(mode, "force")) {
         if (remain >= setting->fullStock) {
            remain = remain;
         }
         else {
            remain = setting->fullStock;
         }
      }

      products[i].id = id;
      products[i].name = copyString(name, strlen(name) + 1); // We add +1 because of the '\0'
      products[i].price = price;
      products[i].remain = remain;
      products[i].sold = sold;

      i++;
   }

   // Write data in the temporaly file.
   fprintf(tmpFile, "%s,%s,%s,%s,%s\n", test.no, test.name, test.price, test.remain, test.sold);

   for (int j = 0; j < i; j++) {
      fprintf(tmpFile, "%d,%s,%.2f,%d,%d\n", j+1, products[j].name, products[j].price, products[j].remain, products[j].sold);
   }

   free(name);
   free(products);
   fclose(file);
   fclose(tmpFile);

   // Delete the old file and rename the temporarily file to the deleted file.
   remove("Data/MockUpProduct.csv");
   rename("Data/__MockUpProduct.csv", "Data/MockUpProduct.csv");

   return;
}

void checkStock(Setting *setting) {

   char* name;
   float price;
   int id, remain, sold;
   int i = 0, flag = 0;
   char choice;

   Header test;
   Product* products;

   name = (char*) malloc(99 * sizeof(char));

   if (name == NULL) {
      printf("Memory allocation failed\n");
      return;
   }

   products = (Product*) malloc(999 * sizeof(Product));

   if (products == NULL) {
      free(name);
      printf("Memory allocation failed\n");
      return;
   }

   FILE *file = fopen("Data/MockUpProduct.csv", "a+");

   float expectLeastStock = (float) ((float) setting->ThresholdPercent / 100.00) * (float) setting->fullStock;

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      return;
   }

   fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,\n]", test.no, test.name, test.price, test.remain, test.sold);

   while (!feof(file)) {
      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      if (expectLeastStock <= (float) remain) continue;
      i++;
   }

   if (i != 0) {
      printf("   There is '%d' product(s) have the amount below threshold (%d%)\n", i, setting->ThresholdPercent);
      printf("   Do you want to restock (y, n): ");
      scanf(" %c", &choice);

      if (choice == 'y' || choice == 'Y') {
         free(name);
         free(products);
         fclose(file);

         printf("Restocking...\n");
         Restock(setting, "normal");
      }
   }
   else {
      printf("There is no product that have the amount below threshold. (%d%)\n", setting->ThresholdPercent);
   }

   free(name);
   free(products);
   fclose(file);

}


void autoRestock(Setting *setting) {

   time_t current_time;
   time(&current_time);

   struct tm *tm_se = localtime(&setting->lastCheck);

   // printf("Setting last check: %d-%d-%d %d:%d:%d\n", tm_se->tm_year + 1900, tm_se->tm_mon + 1, tm_se->tm_mday, tm_se->tm_hour, tm_se->tm_min, tm_se->tm_sec);

   struct tm *tm_local = localtime(&current_time);

   // printf("Current time: %d-%d-%d %d:%d:%d\n", tm_local->tm_year + 1900, tm_local->tm_mon + 1, tm_local->tm_mday, tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec);

   // delay(10);

   if (isTimePassed(setting->lastCheck, 1)) {
      printf("Checking the stock...\n");
      checkStock(setting);
      setting->lastCheck = time(NULL);
      updateSetting(setting);
      // delay(1);
      printf("Done!\n");
   }
}


// Setting Handler
int checkSetting(Setting *targetSetting) {
   /*
      This function use to check if the setting is already set

      return 1 if the process is fail
      return 0 if the process success

      and also if the setting exist, it should update the setting into the given variable.

   */
   Header test;
   Setting tmp;

   long time;

   int ctr = 0;

   FILE *file = fopen("Data/Setting.csv", "a+");

   if (ferror(file) || file == NULL) {
      printf("%s\n", strerror(errno));
      fclose(file);

      return 1;
   }

   fscanf(file, "%[^,],%[^,],%[^,\n]\n", test.no, test.name, test.price);

   while (!feof(file)) {
      fscanf(file, "%d,%d,%ld\n", &tmp.ThresholdPercent, &tmp.fullStock, &time);
      ctr++;
   }

   if (ctr < 1) {
      fclose(file);
      return 1;
   }

   targetSetting->fullStock = tmp.fullStock;
   targetSetting->ThresholdPercent = tmp.ThresholdPercent;
   targetSetting->lastCheck = time;

   fclose(file);

   return 0;

}

int createSetting(Setting setting) {
   /*
      This function use to create the new setting.

      *CAUTION* : This function will overwrite the exist setting.

      return 1 if the process is fail
      return 0 if the process success

   */

   int ctr = 0;

   FILE *file = fopen("Data/Setting.csv", "a+");

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return 1;
   }

   time_t currentTime;

   // Checking if the content in the file is empty
   time(&currentTime);

   FILE *tmpFile = fopen("Data/__Setting.csv", "w+");

   // Write the provided setting into the file
   fprintf(tmpFile, "%s,%s,%s\n", "Threshold", "Fullstock", "Last check");


   fprintf(tmpFile, "%d,%d,%ld\n", setting.ThresholdPercent, setting.fullStock, (long) currentTime);

   fclose(file);
   fclose(tmpFile);

   // Delete the old file and rename the temporarily file to the deleted file.
   remove("Data/Setting.csv");
   rename("Data/__Setting.csv", "Data/Setting.csv");

   return 0;
}

int updateSetting(Setting* setting) {
   /*
      This function use to update the setting.

      return 1 if the process is fail
      return 0 if the process success

   */

   FILE *file = fopen("Data/Setting.csv", "a+");

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return 1;
   }

   time_t currentTime;

   time(&currentTime);

   FILE *tmpFile = fopen("Data/__Setting.csv", "w+");

   // Write the provided setting into the file
   fprintf(tmpFile, "%s,%s,%s\n", "Threshold", "Fullstock", "Last check");

   fprintf(tmpFile, "%d,%d,%ld\n", setting->ThresholdPercent, setting->ThresholdPercent, (long) currentTime);

   fclose(file);
   fclose(tmpFile);

   // Delete the old file and rename the temporarily file to the deleted file.
   remove("Data/Setting.csv");
   rename("Data/__Setting.csv", "Data/Setting.csv");

   return 0;

}


// User Setting Handler
int createUserSetting(UserSetting *setting, char* username) {
   /*
      This function use to create the new user setting.

      return 1 if the process is fail
      return 0 if the process success

   */

   char *filename;

   filename = (char*) malloc(999 * sizeof(char));

   FILE *file;

   time_t currentTime;

   // Checking if the content in the file is empty
   time(&currentTime);

   sprintf(filename, "UserSetting/%s_setting.csv", username);

   if(!isFileExists(filename)) {
      file = fopen(filename, "w+");

      Log("Creating new user setting file for %s", username);

      if (ferror(file) || file == NULL) {
         printf("%s\n", strerror(errno));
         fclose(file);
         return 1;
      }

      fprintf(file, "%s,%s,%s,%s\n", "Auto Purchase Date", "Product Name", "Amount", "Last Purchase", "Last Purchase Date");

      fclose(file);
   }

   FILE *fptr = fopen(filename, "a");

   if (ferror(fptr) || fptr == NULL) {
      printf("%s\n", strerror(errno));
      fclose(fptr);
      return 1;
   }

   Log("Failed to open the user setting file for %s", username);

   Log("%s", setting->product.name);

   // Write the provided setting into the file
   fprintf(fptr, "%d,%s,%d,%ld\n", setting->autoPurchaseDate, setting->product.name, setting->product.remain, (long) currentTime);


   fclose(fptr);


   return 0;
}

int writeCouponAutoPurchase(Coupon coupon, char* username) {
   /*
      This function use to create the new user setting for applying coupon.

      return 1 if the process is fail
      return 0 if the process success

   */

   char *filename;

   filename = (char*) malloc(999 * sizeof(char));

   sprintf(filename, "UserSetting/%s_coupon.csv", username);

   if (isFileExists(filename)) {
      char *choose;

      choose = (char*) malloc(99 * sizeof(char));

      printf("   Coupon already exist, do you want to overwrite it? (y, n): ");
      scanf(" %s", choose);

      if (choose[0] == 'n' || choose[0] == 'N') {
         return 1;
      }

   }

   FILE *file = fopen(filename, "w+");

   if (ferror(file) || file == NULL) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return 1;
   }

   fprintf(file, "%s\n", coupon.code);

   fclose(file);

   return 0;
}

ProductList *LoadUserSettingList(int* ctr, char* username, int* total) {
   /*
      This function use to load the user setting list from the file.

      return nothing
   */

   time_t currentTime;
   Product product;
   char name[999];

   product.name = (char*) malloc(999 * sizeof(char));


   ProductList *productList;

   productList = malloc(sizeof(ProductList));
   productList = NULL;

   // get day of the week
   time(&currentTime);

   struct tm *tm_local = localtime(&currentTime);
   int day = tm_local->tm_wday;
   char *filename;

   filename = (char*) malloc(999 * sizeof(char));

   sprintf(filename, "UserSetting/%s_setting.csv", username);
   Log("Opening %s", filename);

   FILE *file = fopen(filename, "r");

   if (file == NULL) {
      printf("%s\n", strerror(errno));
      return productList;
   }

   long time;
   int dataDay;

   if (file == NULL) {
      printf("%s\n", strerror(errno));
      return productList;
   }

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return productList;
   }

   // Skip the header line
   char header[256];
   if (fgets(header, sizeof(header), file) == NULL) {
      perror("Error reading header");
      fclose(file);
      return productList;
   }

   // delay(1000);

   // Loop until EOF
   while (!feof(file)) {
      Product product;

      char name[999];
      int dataDay;

      product.name = (char*) malloc(999 * sizeof(char));

      fscanf(file, "%d,%[^,],%d,%ld\n", &dataDay, name, &product.remain, &time);

      product.name = copyString(name, strlen(name) + 1);

      if (dataDay != day || !isTimePassed((time_t) time, 1)) continue;

      productList = AppendOrEditProduct(productList, product, ctr, "loadSetting");

      free(product.name);

   }

   fclose(file);

   return productList;
}

void updateAutoPurchase(ProductList *products, char* username) {
   /*
      Purchase provided product by given the name

      return 1 if the process is fail
      return 0 if the process success

   */

   // Use remain form the ProductList as the amount of the product that the user want to purchase.
   ProductList* temp = products;

   char *filename;
   char *tmpfile;

   filename = (char*) malloc(999 * sizeof(char));
   tmpfile = (char*) malloc(999 * sizeof(char));

   sprintf(filename, "UserSetting/%s_setting.csv", username);
   sprintf(tmpfile, "UserSetting/__%s_setting.csv", username);

   float price;
   int id, remain, sold;
   int i = 0, flag = 0;

   Header test;

   UserSettingList* writeSetting;

   writeSetting = (UserSettingList*) malloc(999 * sizeof(UserSettingList));

   if (writeSetting == NULL) {
      printf("Memory allocation failed\n");
      return;
   }

   FILE *file = fopen(filename, "r");

   FILE *tmpFile = fopen(tmpfile, "w+");


   // Checking if unable to open file.

   if (tmpFile == NULL || file == NULL) {
      printf("Cant open file\n");
      return;
   }

   if (ferror(file) || ferror(tmpFile)) {
      printf("%s\n", strerror(errno));
      return;
   }

   char header[256];
   if (fgets(header, sizeof(header), file) == NULL) {
      perror("Error reading header");
      fclose(file);
      return;
   }

   while (!feof(file)) {
      int dataDay;
      Product product;
      long Time;

      time_t currentTime;

      time(&currentTime);

      product.name = (char*) malloc(999 * sizeof(char));

      fscanf(file, "%d,%[^,],%d,%ld\n", &dataDay, product.name, &product.remain, &Time);

      Log("//Product '%s' has been purchased.", product.name);

      temp = products;
      flag = 0;

      while (temp != NULL) {

         Log("//(tmp) Product '%s' has been purchased.", temp->product.name);

         if (!strcmp(temp->product.name, product.name)) {

            writeSetting[i].setting.autoPurchaseDate = dataDay;
            writeSetting[i].setting.product.name = copyString(product.name, strlen(product.name) + 1); // We add +1 because of the '\0'
            writeSetting[i].setting.product.remain = product.remain;
            writeSetting[i].setting.lastPurchase = (long) currentTime;

            i++;

            temp = temp->next;

            flag = 1;
            break;
         }

         temp = temp->next;
      }

      if (!flag) {
         // Update the data
         writeSetting[i].setting.autoPurchaseDate = dataDay;
         writeSetting[i].setting.product.name = copyString(product.name, strlen(product.name) + 1); // We add +1 because of the '\0'
         writeSetting[i].setting.product.remain = product.remain;
         writeSetting[i].setting.lastPurchase = (long) Time;

         i++;
      }
   }

   //Write data in the temporaly file.
   fprintf(tmpFile, "%s,%s,%s,%s\n", "Auto Purchase Date", "Product Name", "Last Purchase Date", "Last Purchase Date");

   for (int j = 0; j < i; j++) {
      fprintf(tmpFile, "%d,%s,%d,%ld\n", writeSetting[j].setting.autoPurchaseDate, writeSetting[j].setting.product.name, writeSetting[j].setting.product.remain, writeSetting[j].setting.lastPurchase);
   }

   free(writeSetting);
   fclose(file);
   fclose(tmpFile);

   // Delete the old file and rename the temporarily file to the deleted file.
   remove(filename);
   rename(tmpfile, filename);

   return;
}

void autoPurchase(char *username) {
   /*
      This function use to auto purchase the product that have the amount below
      the threshold

      return nothing
   */

   Log("Attempting to auto purchase");

   ProductList *productList;

   int totalProduct = 0;

   productList = malloc(sizeof(ProductList));

   if (productList == NULL) {
      perror("Failed to allocate memory");
   }

   productList = NULL;

   int ctr = 0;

   productList = LoadUserSettingList(&ctr, username, &totalProduct);

   char *filename;

   filename = (char*) malloc(999 * sizeof(char));

   sprintf(filename, "UserSetting/%s_coupon.csv", username);

   // Log("%s", productList->product.name);

   Log("Loaded user setting list");

   if (ctr <= 0) {
      clearScreen();
      printf("There is no product to auto purchase today.\n");
      delay(2);
      clearScreen();

      return;
   }

   double totalPrice = 0;

   purchaseMultipleProductWithName(productList, &totalPrice);

   printf("Auto purchase done!\n");

   if (isFileExists(filename)) {
      char *couponCode;

      couponCode = (char*) malloc(999 * sizeof(char));

      if (couponCode == NULL) {
         perror("Failed to allocate memory");
      }

      FILE *file = fopen(filename, "r");

      if (file == NULL) {
         printf("%s\n", strerror(errno));
         return;
      }

      Coupon coupon = getCouponByCode(couponCode);

      if (coupon.id != -1) {

         if (!strcmp(coupon.type, "PERCENTAGE")) {
            totalPrice -= (totalPrice * coupon.discount / 100);
            updateDailySummary(totalPrice, totalProduct, "coupon");
         }
         else if (!strcmp(coupon.type, "MINIMUM")) {
            if (totalPrice < coupon.minAmount) {
               totalPrice -= coupon.discount;
               updateDailySummary(totalPrice, totalProduct, "coupon");
            }
            else {
               clearScreen();
               delay(1);
               printf("Coupon can't be applied\n");
               Log("Coupon can't be applied because the total price is below the minimum amount");
               updateDailySummary(totalPrice, totalProduct, "no coupon");
            }
         }
      }
      else {
         updateDailySummary(totalPrice, totalProduct, "no coupon");
      }
   }

   delay(1);
   clearScreen();
   printf("Purchase %d products in total price: %.2f\n", ctr, totalPrice);
   delay(3);
   clearScreen();
   Log("Auto purchase done!");
   updateAutoPurchase(productList, username);
   clearScreen();

   return;
}


// Unused function
UserSettingList* AppendAndEditUserSettingList(UserSettingList *userSettingList, UserSetting userSetting) {
   /*
      This function use to append the user setting into the list.

      return the new list
   */

   UserSettingList *head = userSettingList;
   int flag = 0;


   // Loop until the end of the list
   while (userSettingList != NULL) {
      if (strcmp(userSettingList->setting.product.name, userSetting.product.name) == 0) {
            userSettingList->setting.product.remain += userSetting.product.remain;
            flag = 1;
            break;
      }
      if (userSettingList->next == NULL) break;
      userSettingList = userSettingList->next;
   }

   if (!flag) {

      UserSettingList *newSettingUserList = malloc(sizeof(UserSetting));
      if (newSettingUserList == NULL) {
         perror("Failed to allocate memory");
      }

      // Increase the product count by 1 for every new product

      newSettingUserList->setting = userSetting;
      newSettingUserList->next = NULL;

      if (userSettingList != NULL) {
         userSettingList->next = newSettingUserList;
      }
      else {
         head = newSettingUserList;
      }
   }

   return head;
}

void updateDailySummary(float Price, int amount, char *mode) {
   /*
      This function use to update the daily summary

      return nothing
   */

   float earningWithoutCoupon = 0;
   float earningWithCoupon = 0;
   float totalEarning = 0;
   int totalProductSold = 0;

   time_t currentTime;
   time(&currentTime);

   struct tm *tm_local = localtime(&currentTime);

   char *dirDate = malloc(100 * sizeof(char));
   char *tmpDirdate = malloc(100 * sizeof(char));

   int day = tm_local->tm_mday;
   int month = tm_local->tm_mon + 1;
   int year = tm_local->tm_year + 1900;

   sprintf(dirDate, "Report/%02d-%02d-%04d_report.csv", day, month, year);
   sprintf(tmpDirdate, "Report/__%02d-%02d-%04d_report.csv", day, month, year);

   FILE *tmpFile = fopen(tmpDirdate, "w+");
   FILE *file = fopen(dirDate, "r");

   if (ferror(file) || file == NULL || ferror(tmpFile) || tmpFile == NULL) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return;
   }


   if (ferror(file) || ferror(tmpFile)) {
      printf("%s\n", strerror(errno));
      fclose(file);
      fclose(tmpFile);
      return;
   }

   char header[256];
   if (fgets(header, sizeof(header), file) == NULL) {
      perror("Error reading header");
      fclose(file);
      return;
   }


   fscanf(file, "%f,%f,%f,%d\n", &earningWithoutCoupon, &earningWithCoupon, &totalEarning, &totalProductSold);

   Log("Earning without coupon: %.2f", earningWithoutCoupon);
   Log("Earning with coupon: %.2f", earningWithCoupon);
   Log("Total Earning: %.2f", totalEarning);
   Log("Total Product Sold: %d", totalProductSold);

   if (!strcmp(mode, "coupon")) {
      earningWithCoupon += Price;
   }
   else {
      earningWithoutCoupon += Price;
   }

   totalEarning += Price;
   totalProductSold += amount;


   fprintf(tmpFile, "%s,%s,%s,%s\n", "Earning without coupon", "Earning with coupon", "Total Earning", "Total Product Sold");

   fprintf(tmpFile, "%.2f,%.2f,%.2f,%d\n", earningWithoutCoupon, earningWithCoupon, totalEarning, totalProductSold);

   fclose(file);
   fclose(tmpFile);

   // Delete the old file and rename the temporarily file to the deleted file.
   remove(dirDate);
   rename(tmpDirdate, dirDate);

   return;
}

void displayDailySummary(int day, int month, int year) {
   /*
      This function use to display the daily summary

      return nothing
   */

   time_t currentTime;
   time(&currentTime);

   struct tm *tm_local = localtime(&currentTime);

   int d = tm_local->tm_mday;
   int m = tm_local->tm_mon + 1;
   int y = tm_local->tm_year + 1900;

   if (day == 0 || month == 0 || year == 0) {
      printf("Invalid date\n");
      printf("Press enter to continue...\n");
      getch();
      clearScreen();
      delay(1);
      return;
   }

   if (day > 31 || month > 12 || year < 1900) {
      printf("Invalid date\n");
      printf("Press enter to continue...\n");
      getch();
      clearScreen();
      delay(1);
      return;
   }

   if (day == d && month == m && year == y) {
      printf("There's no report for today yet.\n");
      printf("Press enter to continue...\n");
      getch();
      clearScreen();
      delay(1);
      return;
   }


   delay(1);
   clearScreen();

   char *dirDate = malloc(100 * sizeof(char));

   sprintf(dirDate, "Report/%02d-%02d-%04d_report.csv", day, month, year);

   if (!isFileExists(dirDate)) {
      printf("There is no report for %02d-%02d-%04d\n", day, month, year);
      printf("Press enter to continue...\n");
      getch();
      clearScreen();
      delay(1);
      return;
   }

   FILE *file = fopen(dirDate, "r");

   if (ferror(file) || file == NULL) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return;
   }

   char header[256];
   if (fgets(header, sizeof(header), file) == NULL) {
      perror("Error reading header");
      fclose(file);
      return;
   }

   float earningWithoutCoupon = 0;
   float earningWithCoupon = 0;
   float totalEarning = 0;
   int totalProductSold = 0;

   fscanf(file, "%f,%f,%f,%d\n", &earningWithoutCoupon, &earningWithCoupon, &totalEarning, &totalProductSold);

   borderup();
   printf("   Earning without coupon: %.2f THB.\n", earningWithoutCoupon);
   printf("   Earning with coupon: %.2f THB.\n", earningWithCoupon);
   printf("   Total Earning: %.2f THB.\n", totalEarning);
   printf("   Total Product Sold: %d Items\n", totalProductSold);
   borderdown();
   printf("Press enter to continue...");
   getch();
   clearScreen();
   delay(1);
}
