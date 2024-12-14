#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void delay(int second){
   // time function for delaying
   int millisec = 1000 * second;

   clock_t start_time = clock();

   while(clock()<start_time + millisec);
}

void borderup(){
   // Upper border for UI
   for(int i = 0; i < 3; i++){
      for(int j = 0; j <= 20; j++){
         if(i == 0 || j == 0 || j == 20){
            printf("* ");
         }else{
            printf("  ");
         }
      }
      printf("\n");
   }
}

void borderdown(){
   // Lower border for UI
   for(int i = 0; i < 3; i++){
      for(int j = 0; j <= 20; j++){
         if(i == 2 || j == 0 || j == 20){
            printf("* ");
         }else{
            printf("  ");
         }
      }
      printf("\n");
   }
}

void clearScreen() {
   system("@cls||clear");

   return;
}
