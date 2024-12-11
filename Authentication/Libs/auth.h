typedef struct User {
   char* username;
   char* password;
   char* role;
} User;

int login(char *username, char *password, User* user);

int reg(char *username, char *password, char *role);

int logout(User* user);
