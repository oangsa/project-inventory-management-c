#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "couponHandler.h"

// ตัวแปรสำหรับติดตาม ID คูปองล่าสุด
static int couponIdCounter = 1; // เริ่มต้นจาก ID 1

// สร้างคูปองใหม่
int createCoupon(Coupon *coupon, const char *code, float discount, time_t expirationDate) {
    if (!coupon || !code) return 1;

    coupon->id = couponIdCounter++; // ใช้ตัวแปร couponIdCounter เพื่อเพิ่ม ID
    strncpy(coupon->code, code, sizeof(coupon->code) - 1);
    coupon->discount = discount;
    coupon->expirationDate = expirationDate;

    return 0;
}
