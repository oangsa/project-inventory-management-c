#include <stdio.h>
#include <time.h>
#include "couponHandler.h"

// สมมติว่าเพื่อนมีฟังก์ชันนี้ในระบบ CRUD
extern void applyCouponFromAnotherSystem(Coupon coupon); 

int main() {
    Coupon coupon;

    // สร้างคูปอง
    time_t expirationDate = time(NULL) + 7 * 24 * 3600; // หมดอายุใน 7 วัน
    if (createCoupon(&coupon, "DISCOUNT10", 10.0, expirationDate) == 0) {
        printf("Coupon created: ID=%d, Code=%s, Discount=%.2f%%\n", coupon.id, coupon.code, coupon.discount);

        // ส่งค่า Coupon ไปให้เพื่อนโดยตรง
        applyCouponFromAnotherSystem(coupon);
    } else {
        printf("Failed to create coupon.\n");
    }

    return 0;
}

// debugged By ChatGPT Kung na ja