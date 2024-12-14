#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>


void printLog(const char *msg) {
   time_t current_time;
   current_time = time(NULL);

   struct tm *tm_local = localtime(&current_time);

   FILE *file = fopen("log.txt", "a");

   if (file == NULL) {
      printf("Error opening file!\n");
      return;
   }

   fprintf(file, "[%d:%d:%d] %s\n",tm_local -> tm_hour, tm_local -> tm_min, tm_local -> tm_sec, msg);

   free(file);
   fclose(file);
}

void Log(const char *fmt, ...) {
   // determine required buffer size
   va_list args;
   va_start(args, fmt);
   int len = vsnprintf(NULL, 0, fmt, args);
   va_end(args);
   if(len < 0) return;

   // format message
   char msg[len + 1]; // or use heap allocation if implementation doesn't support VLAs
   va_start(args, fmt);
   vsnprintf(msg, len + 1, fmt, args);
   va_end(args);

   // call printLog with formatted message
   printLog(msg);
   }
