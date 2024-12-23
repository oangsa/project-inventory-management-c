#ifndef COUPON_H
#define COUPON_H

#include <time.h>

// Struct for a Coupon
typedef struct {
    int id;
    char code[20];
    float discount;
    time_t expirationDate;
    char type[12]; // เกเป็น % หรือ ขั้นต่ำ
    float minAmount;
} Coupon;

// Function prototypes
int displayCoupons(const char *filename);
void addCoupon(const char *filename);
void deleteCoupon(const char *filename);
void selectCoupon(const char *filename);

int getTotalCoupons(const char *filename);

Coupon getCouponByCode(char *couponCode);

#endif
