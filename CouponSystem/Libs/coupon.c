#include "coupon.h"
#include "../../LogSystem/LogHandler.h"
#include "../../Authentication/Libs/utils.h"
#include "../../CRUD/Libs/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Display all coupons
void displayCoupons(const char *filename) {
   Log("Display coupon triggered.");
   char header[256];

   char buffer[26];
   char type[20] = {0};
   float minAmount = 0.0;

   Coupon tmp;
   int isNoCoupon = 1;

   FILE *file = fopen(filename, "r"); // Use "r" since you’re reading data
   if (!file) {
      perror("Error opening file");
      return;
   }

   // Read and log the header line
   if (fgets(header, sizeof(header), file) == NULL) {
      perror("Error reading header");
      fclose(file);
      return;
   }

   // Read and display coupon data
   printf("+-----------------------------------------------[Coupons]------------------------------------------------+\n");
   printf("| Id  |             Code             |   Discount Price    |   Type   |    Minimum    |  Expiration Date |\n");
   printf("|-----|------------------------------|---------------------|----------|---------------|------------------|\n");

   while (fscanf(file, "%d,%[^,],%f,%ld,%[^,],%f\n",
               &tmp.id, tmp.code, &tmp.discount, &tmp.expirationDate, type, &minAmount) == 6) {
      isNoCoupon = 0;

      strftime(buffer, 26, "%Y-%m-%d", localtime(&tmp.expirationDate));

      if (strcmp(type, "PERCENTAGE") == 0) {
         printf("|  %-3d| %-28s | %3.2f%2s             | %2s%-6s | %-13s |    %-13s |\n", tmp.id, tmp.code, tmp.discount, "%", " ", "PER", "-", buffer);
         // printf("| %-3d | %-28s | %-19.2f THB | %-8s | %-13s | %-16s |\n", tmp.id, tmp.code, tmp.discount, "PER", "-", buffer);
      } else if (strcmp(type, "MINIMUM") == 0) {
         printf("|  %-3d| %-28s | %3.2f%2s             | %2s%-6s | %3.2f%4s    |    %-13s |\n", tmp.id, tmp.code, tmp.discount, "%", " ", "MIN", minAmount, "THB", buffer);
         // printf("| %-3d | %-28s | %-19.2f THB | %-8s | %-13.2f THB | %-16s |\n", tmp.id, tmp.code, tmp.discount, "MIN", minAmount, buffer);
      } else {
         printf("| %-3d | %-28s | %-19.2f THB | %-8s | %-13s | %-16s |\n", tmp.id, tmp.code, tmp.discount, "UKN", "-", buffer);
      }
   }

   if (isNoCoupon) {
      printf("|                                          No Coupon available.                                          |\n");
   }

   printf("+--------------------------------------------------------------------------------------------------------+\n");

   fclose(file);
}



int getNextId(const char *filename) {
   FILE *file = fopen(filename, "r");
   if (!file) {
      // If file doesn't exist or is empty, start with ID = 1
      return 1;
   }

   int maxId = 0;
   char line[256];

   while (fgets(line, sizeof(line), file)) {
      Coupon temp;
      sscanf(line, "%d,%19[^,],%f,%ld", &temp.id, temp.code, &temp.discount, &temp.expirationDate);
      if (temp.id > maxId) {
         maxId = temp.id;
      }
   }

   fclose(file);
   return maxId + 1;
}

// Add a new coupon
void addCoupon(const char *filename) {
   Log("Add coupon triggered.");
   char code[20];
   float discount, minAmount = 0;
   int validDays, choice;
   Coupon newCoupon;

   while (1) {
      borderup();
      printf("   Enter coupon code: ");
      scanf("%s", code);

      printf("   Choose coupon type:\n");
      printf("      1. Percentage Discount\n");
      printf("      2. Minimum Purchase Discount\n");
      printf("   Enter your choice (1 or 2): ");
      scanf("%d", &choice);

      switch(choice) {
         case 1:
            printf("   Enter discount percentage: ");
            scanf("%f", &discount);
            if(discount <= 0 || discount > 100) {
                  printf("   Invalid discount percentage! Try again.\n");
                  delay(1);
                  break;
            }
            strcpy(newCoupon.type, "PERCENTAGE");
            newCoupon.discount = discount;
            newCoupon.minAmount = 0; // Not applicable for percentage type
            break;

         case 2:
            printf("   Enter minimum amount: ");
            scanf("%f", &minAmount);
            printf("   Enter discount amount: ");
            scanf("%f", &discount);
            if( minAmount <= 0 || discount <= 0 ) {
               printf("   Invalid values! Try again..\n");
               delay(1);
               break;
            }
            strcpy(newCoupon.type, "MINIMUM");
            newCoupon.discount = discount;
            newCoupon.minAmount = minAmount;
            break;

         default:
            printf("   Invalid choice! Try again..\n");
            delay(1);
            break;
      }

      printf("   Enter validity in days: ");
      scanf("%d", &validDays);

      newCoupon.id = getNextId(filename);
      strcpy(newCoupon.code, code);
      newCoupon.expirationDate = time(NULL) + validDays * 24 *60 *60;
      break;
   }


   FILE *file = fopen(filename, "a");
   if (!file) {
      perror("Error opening file");
      return;
   }

   fprintf(file, "%d,%s,%.2f,%ld,%s,%.2f\n", newCoupon.id, newCoupon.code, newCoupon.discount,
         newCoupon.expirationDate, newCoupon.type, newCoupon.minAmount);
   fclose(file);

   printf("   Coupon added successfully! ID: %d\n\n", newCoupon.id);
   Log("Coupon '%s' has been created.", newCoupon.code);
   borderdown();
   delay(2);
}

// Delete a coupon by ID
void deleteCoupon(const char *filename) {
   Log("Delete coupon triggered.");
   int idToDelete;


   borderup();
   printf("   Enter the ID of the coupon to delete: ");
   scanf("%d", &idToDelete);

   FILE *file = fopen(filename, "r");
   if (!file) {
      perror("   Error opening file");
      borderdown();
      delay(2);
      return;
   }

   FILE *tempFile = fopen("Data/__Coupon.csv", "w");
   if (!tempFile) {
      perror("   Error creating temporary file");
      fclose(file);
      return;
   }

   char line[256];
   int id;
   int isDeleted = 0;

   while (fgets(line, sizeof(line), file)) {
      sscanf(line, "%d,", &id);
      if(id == idToDelete) {
         Log("Coupon with ID %d deleted.", idToDelete);
         printf("   Coupon with ID %d deleted successfully.\n", idToDelete);
         isDeleted = 1;
      }else {
         fputs(line, tempFile);
      }
   }

   fclose(file);
   fclose(tempFile);

   remove(filename);
   rename("Data/__Coupon.csv", filename);

   if (!isDeleted) {
      printf("   No coupon with ID %d found.\n", idToDelete);
      Log("Unable to delete coupon with ID %d.", idToDelete);
   }
   else {
      borderdown();
      delay(1);
   }
}

// Select a coupon
void selectCoupon(const char *filename) {
   int idToSelect;
   printf("Enter the ID of the coupon to select: ");
   scanf("%d", &idToSelect);

   FILE *file = fopen(filename, "r");
   if (!file) {
      perror("Error opening file");
      return;
   }

   char line[256];
   Coupon temp;
   char type[20];
   float minAmount = 0.0;
   int found = 0;

   while (fgets(line, sizeof(line), file)) {
      if (line[0] == '\n' || line[0] == '\0') continue; // ข้ามบรรทัดว่าง

      // อ่านข้อมูลจากบรรทัด
      int fieldsRead = sscanf(line, "%d,%19[^,],%f,%ld,%19[^,],%f",
                              &temp.id, temp.code, &temp.discount,
                              &temp.expirationDate, type, &minAmount);

      if (fieldsRead < 4) { // ถ้าข้อมูลไม่สมบูรณ์ ข้ามไป
         printf("Warning: Skipped malformed line -> %s", line);
         continue;
      }

      // ตรวจสอบว่า ID ตรงกับที่ต้องการหรือไม่
      if (temp.id == idToSelect) {
         char buffer[26];
         strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", localtime(&temp.expirationDate));

         printf("\nSelected Coupon Details:\n");
         printf("ID: %d\n", temp.id);
         printf("Code: %s\n", temp.code);
         printf("Discount: %.2f\n", temp.discount);
         printf("Type: %s\n", type);

         if (strcmp(type, "MINIMUM") == 0) {
               printf("Min Amount: %.2f\n", minAmount);
         } else {
               printf("Min Amount: -\n");
         }

         printf("Expiration Date: %s\n", buffer);
         found = 1;
         break; // เจอแล้วหยุดการค้นหา
      }
   }

   if (!found) {
      printf("Coupon with ID %d not found.\n", idToSelect);
   }

   fclose(file);
}

// Get a coupon by name
Coupon getCouponByCode(char* couponCode) {
   Coupon coupon;
   FILE *file = fopen("Data/Coupon.csv", "r");
   if (!file) {
      perror("Error opening file");
      return coupon;
   }

   char line[256];
   char type[20];
   float minAmount = 0.0;

   while (fgets(line, sizeof(line), file)) {
      if (line[0] == '\n' || line[0] == '\0') continue; // ข้ามบรรทัดว่าง

      // อ่านข้อมูลจากบรรทัด
      int fieldsRead = sscanf(line, "%d,%19[^,],%f,%ld,%19[^,],%f", &coupon.id, coupon.code, &coupon.discount, &coupon.expirationDate, coupon.type, &minAmount);

      if (fieldsRead < 4) continue;

      // ตรวจสอบว่า ID ตรงกับที่ต้องการหรือไม่
      if (strcmp(coupon.code, couponCode) == 0) {
         fclose(file);
         return coupon;
      }
   }

   coupon.id = -1;

   fclose(file);
   return coupon;
}
