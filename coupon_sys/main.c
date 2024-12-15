#include "lib/coupon.h"
#include <stdio.h>

int main() {
    const char *filename = "coupons.csv";
    int choice;

    do {
        printf("Coupon Management System\n");
        printf("-------------------------\n");
        printf("1. Display all coupons\n");
        printf("2. Add a new coupon\n");
        printf("3. Delete a coupon\n");
        printf("4. Select a coupon\n");
        printf("5. Edit a coupon\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayCoupons(filename);
                break;
            case 2:
                addCoupon(filename);
                break;
            case 3:
                displayCoupons(filename);
                deleteCoupon(filename);
                break;
            case 4:
                selectCoupon(filename);
                break;
            case 5:
                editCoupon(filename);
                break;
            case 6:
                printf("Exiting... Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n\n");
        }
    } while (choice != 5);

    return 0;
}
