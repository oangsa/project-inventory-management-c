#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "utils.h"

// Just structure of the header to prevent the confusion.
typedef struct Header {
   char no[99];
   char name[99];
   char price[99];
   char remain[99];
   char sold[99];
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
   int id, remain, sold;

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
         printf("---------------------------------[Products]----------------------------------\n");
         printf("| %-2s  | %16s%-13s|%9s%-3s|%9s%-3s|%8s%-4s|\n", "Id", test.name," ", test.price, " ", test.remain, " ", test.sold, " ");
         printf("|%s|%16s|%12s|%8s|%6s|\n", "-----", "------------------------------", "------------", "------------", "------------");
         isNoProduct = 0;
      }
      printf("|  %-2d | %-28s | %-7.2f%s | %-4d%6s | %-4d%6s |\n", id, name, price, "THB", remain, "Items", sold, "Items");

      fscanf(file, "%d,%[^,],%f,%d,%d\n", &id, name, &price, &remain, &sold);

      if (remain == 0 && strcmp(mode, "customer") == 0) continue;

   }

   if (!isNoProduct) {
      printf("|  %-2d | %-28s | %-7.2f%s | %-4d%6s | %-4d%6s |\n", id, name, price, "THB", remain, "Items", sold, "Items");
      printf("-----------------------------------------------------------------------------\n");
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

   // while (temp != NULL) {
   //    printf("   Id: %d\n", temp->product.id);
   //    printf("   Remain: %d\n", temp->product.remain);

   //    temp = temp->next;
   // }

   // delay(10);

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

         if (temp->product.id == id) {

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

void Restock(Setting *setting) {
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

      if (expectLeastStock > (float) remain) {
         remain = setting->fullStock;
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
         Restock(setting);
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
int createUserSetting(UserSetting *setting) {
   /*
      This function use to create the new user setting.

      return 1 if the process is fail
      return 0 if the process success

   */

   FILE *file = fopen("Data/UserSetting.csv", "a+");

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return 1;
   }

   // Write the provided setting into the file
   fprintf(file, "%d,%s,%d\n", setting->autoPurchaseDate, setting->product.name, setting->product.remain);

   fclose(file);


   return 0;
}

void LoadUserSettingList(ProductList *productList, int* ctr) {
   /*
      This function use to load the user setting list from the file.

      return nothing
   */

   time_t currentTime;

   // get day of the week
   time(&currentTime);

   struct tm *tm_local = localtime(&currentTime);

   int day = tm_local->tm_wday;

   FILE *file = fopen("Data/UserSetting.csv", "r");

   productList = NULL;

   Header test;

   if (file == NULL) {
      printf("%s\n", strerror(errno));
      return;
   }

   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return;
   }

   // Get rid of the header
   fscanf(file, "%[^,],%[^,],[^,\n]\n", test.no, test.name, test.price);

   // Loop until EOF
   while (!feof(file)) {
      Product product;
      int dataDay;

      fscanf(file, "%d,%[^,],%d\n", &dataDay, product.name, &product.remain);

      if (dataDay != day) continue;

      productList = AppendOrEditProduct(productList, product, ctr, "loadSetting");
   }

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
