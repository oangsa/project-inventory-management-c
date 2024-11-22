#include <time.h>

// โครงสร้างคูปอง
typedef struct Coupon {
    int id;
    char code[20];        // รหัสคูปอง
    float discount;       // ส่วนลด (%)
    time_t expirationDate; // วันหมดอายุ
} Coupon;

// สร้างคูปองใหม่
int createCoupon(Coupon *coupon, const char *code, float discount, time_t expirationDate);
