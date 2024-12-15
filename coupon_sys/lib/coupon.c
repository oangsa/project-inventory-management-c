#include "coupon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Display all coupons
void displayCoupons(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    printf("\nAll Coupons:\n");
    printf("ID\tCode\t\tDiscount\tType\t\tMin Amount\tExpiration Date\n");
    printf("------------------------------------------------------------------------\n");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // ข้ามบรรทัดว่าง
        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }

        Coupon temp;
        char type[20] = {0}; // เก็บค่า Type ชั่วคราว
        float minAmount = 0.0;

        // ใช้ sscanf อ่านข้อมูลจากบรรทัด
        int fieldsRead = sscanf(line, "%d,%19[^,],%f,%ld,%19[^,],%f",
                                &temp.id, temp.code, &temp.discount,
                                &temp.expirationDate, type, &minAmount);

        if (fieldsRead < 4) { // ถ้าฟิลด์ไม่ครบ 4 ช่อง ถือว่าข้อมูลไม่ถูกต้อง
            printf("Error reading line: %s", line);
            continue;
        }

        char buffer[26];
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", localtime(&temp.expirationDate));

        // แสดงผลลัพธ์ตาม Type
        if (strcmp(type, "PERCENTAGE") == 0) {
            printf("%d\t%s\t%.2f%%\t\t%s\t\t-\t\t%s\n",
                   temp.id, temp.code, temp.discount, type, buffer);
        } else if (strcmp(type, "MINIMUM") == 0) {
            printf("%d\t%s\t%.2f\t\t%s\t\t%.2f\t\t%s\n",
                   temp.id, temp.code, temp.discount, type, minAmount, buffer);
        } else {
            printf("%d\t%s\t%.2f\t\tUnknown\t\t-\t\t%s\n",
                   temp.id, temp.code, temp.discount, buffer);
        }
    }

    fclose(file);
    printf("------------------------------------------------------------------------\n");
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
    char code[20];
    float discount, minAmount = 0;
    int validDays, choice;

    printf("Enter coupon code: ");
    scanf("%s", code);

    printf("Choose coupon type:\n");
    printf("1. Percentage Discount\n");
    printf("2. Minimum Purchase Discount\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    Coupon newCoupon;

    switch(choice) {
        case 1:
            printf("Enter discount percentage: ");
            scanf("%f", &discount);
            if(discount <= 0 || discount > 100) {
                printf("Invalid discount percentage! Coupon not added.\n");
                return;
            }
            strcpy(newCoupon.type, "PERCENTAGE");
            newCoupon.discount = discount;
            newCoupon.minAmount = 0; // Not applicable for percentage type
            break;
        
        case 2:
            printf("Enter minimum amount for the discount: ");
            scanf("%f", &minAmount);
            printf("Enter discount amount: ");
            scanf("%f", &discount);
            if(minAmount <= 0 || discount <= 0) {
                printf("Invalid values! Coupon not added.\n");
                return;
            }
            strcpy(newCoupon.type, "MINIMUM");
            newCoupon.discount = discount;
            newCoupon.minAmount = minAmount;
            break;
        
        default:
            printf("Invalid choice! Coupon not added.\n");
            return;
    }

    printf("Enter validity in days: ");
    scanf("%d", &validDays);

    newCoupon.id = getNextId(filename);
    strcpy(newCoupon.code, code);
    newCoupon.expirationDate = time(NULL) + validDays * 24 *60 *60;

    FILE *file = fopen(filename, "a");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%d,%s,%.2f,%ld,%s,%.2f\n", newCoupon.id, newCoupon.code, newCoupon.discount,
            newCoupon.expirationDate, newCoupon.type, newCoupon.minAmount);
    fclose(file);

    printf("Coupon added successfully! ID: %d\n\n", newCoupon.id);
}

// Delete a coupon by ID
void deleteCoupon(const char *filename) {
    int idToDelete;

    printf("Enter the ID of the coupon to delete: ");
    scanf("%d", &idToDelete);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    FILE *tempFile = fopen("temp.csv", "w");
    if (!tempFile) {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    char line[256];
    int id;
    int isDeleted = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,", &id);
        if(id == idToDelete) {
            printf("Coupon with ID %d deleted successfully.\n", idToDelete);
            isDeleted = 1;
        }else {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("temp.csv", filename);

    if (!isDeleted) {
        printf("No coupon with ID %d found.\n", idToDelete);
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

// Edit a coupon by ID
void editCoupon(const char *filename) {
    int idToEdit;
    printf("Enter the ID of the coupon to edit: ");
    scanf("%d", &idToEdit);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    FILE *tempFile = fopen("temp.csv", "w");
    if (!tempFile) {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    char line[256];
    int id;
    int isEdited = 0;

    while (fgets(line, sizeof(line), file)) {
        Coupon temp;
        char type[20];
        float minAmount = 0.0;

        // Parse line data
        int fieldsRead = sscanf(line, "%d,%19[^,],%f,%ld,%19[^,],%f",
                                &temp.id, temp.code, &temp.discount,
                                &temp.expirationDate, type, &minAmount);

        if (fieldsRead < 4) { // Skip malformed lines
            fputs(line, tempFile);
            continue;
        }

        if (temp.id == idToEdit) {
            isEdited = 1;
            printf("Editing coupon ID %d:\n", idToEdit);
            printf("Current Code: %s\n", temp.code);
            printf("Enter new code (or press Enter to keep current): ");
            getchar(); // Consume newline
            char newCode[20];
            fgets(newCode, sizeof(newCode), stdin);
            if (newCode[0] != '\n') {
                newCode[strcspn(newCode, "\n")] = 0; // Remove newline
                strcpy(temp.code, newCode);
            }

            printf("Current Discount: %.2f\n", temp.discount);
            printf("Enter new discount (or -1 to keep current): ");
            float newDiscount;
            scanf("%f", &newDiscount);
            if (newDiscount >= 0) {
                temp.discount = newDiscount;
            }

            if (strcmp(type, "MINIMUM") == 0) {
                printf("Current Min Amount: %.2f\n", minAmount);
                printf("Enter new min amount (or -1 to keep current): ");
                float newMinAmount;
                scanf("%f", &newMinAmount);
                if (newMinAmount >= 0) {
                    minAmount = newMinAmount;
                }
            }

            printf("Current Validity (days from now): ");
            time_t currentTime = time(NULL);
            int currentValidity = (temp.expirationDate - currentTime) / (24 * 60 * 60);
            printf("%d days\n", currentValidity);
            printf("Enter new validity (days, or -1 to keep current): ");
            int newValidity;
            scanf("%d", &newValidity);
            if (newValidity >= 0) {
                temp.expirationDate = currentTime + newValidity * 24 * 60 * 60;
            }

            fprintf(tempFile, "%d,%s,%.2f,%ld,%s,%.2f\n",
                    temp.id, temp.code, temp.discount, temp.expirationDate, type, minAmount);
            printf("Coupon ID %d updated successfully.\n", idToEdit);
        } else {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!isEdited) {
        printf("No coupon with ID %d found.\n", idToEdit);
        remove("temp.csv");
    } else {
        remove(filename);
        rename("temp.csv", filename);
    }
}

