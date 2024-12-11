#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "auth.h"
#include "utils.h"
#include "../../CRUD/Libs/utils.h"

typedef struct Header {
    char username[99];
    char password[99];
    char role[99];
} Header;

int login(char* username, char* password, User* user) {
   char Username[99];
   char Password[99];
   char Role[99];

   Header test;

   FILE *file = fopen("Data/users.csv", "r");

   if (!file) {
      perror("Error opening file");
      return 1;
   }

   // Error handling
   if (ferror(file) || !file) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return 1;
   }

   fscanf(file, "%[^,],%[^,],%[^,\n]\n", test.username, test.password, test.role);

   while (!feof(file)) {
      fscanf(file, "%[^,],%[^,],%[^,\n\r]\n", Username, Password, Role);

      if (!strcmp(username, Username) && !strcmp(password, Password)) {
         user->username = copyString(Username, strlen(Username));
         user->password = copyString(Password, strlen(Password));
         user->role = copyString(Role, strlen(Role));

         fclose(file);

         return 0;
      }
   }

   fclose(file);

   return 1;
}

int reg(char* username, char* password, char* role) {
   FILE *file;
   char Username[99];
   char Password[99];
   char Role[99];

   Header test;


   file = fopen("Data/users.csv", "r");

   // Error handling
   if (ferror(file)) {
      printf("%s\n", strerror(errno));
      fclose(file);
      return 1;
   }

   if (!file) {
      perror("Error opening file");
      return 1;
   }

   fscanf(file, "%[^,],%[^,],%[^,\n]\n", test.username, test.password, test.role);

   while (!feof(file)) {
      fscanf(file, "%[^,],%[^,],%[^,\n]\n", Username, Password, Role);

      if (!strcmp(username, Username)) {
         return 1;
      }

   }

   fclose(file);

   file = fopen("Data/users.csv", "a");

   fprintf(file, "%s,%s,%s", username, password, role);

   fclose(file);

   return 0;
}
