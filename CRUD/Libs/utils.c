#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "utils.h"

char* copyString(char *s, int size) {
   /*
      use to copy the string

      return given string for assign to the assignee.
   */
   char* s2;

   s2 = (char*) malloc((size * sizeof(char)) + 1);

   strcpy(s2, s);


   return (char*)s2;
}

int isTimePassed(time_t cmpTime, int day) {
    /*
        Use to check the given time if it pass the given day;

        return 1 if the given time is already passed
        return 0 if the given time haven't pass.

        Example:
            If the given timestamp is on 12/11/2024 00.00
            and the given day is 1.

            Suppose today is 13/11/2024 00.00
            and you call the function

            isTimePassed(oldTime, 1);

            the function should return 1;

            but if the given day is 2, the function would return 0;
    */

    // Change from day to second
    // Day * hrs in day * mins in hr * secs in min
    int expectPass = day * 24 * 60 * 60;
    time_t currentTime;

    time(&currentTime);

    // Compare the current time with the given time;
    double diffTime = difftime(currentTime, cmpTime);

    if ((double) expectPass >= diffTime) return 0;

    return 1;

}
