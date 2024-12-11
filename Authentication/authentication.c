#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
void delay(int second){//time function for delaying
    int millisec = 1000*second;
    clock_t start_time = clock();
    while(clock()<start_time+millisec);
}
int borderup(){//Upper border for UI
    for(int i=0;i<3;i++){
        for(int j=0;j<=20;j++){
                if(i==0||j==0||j==20){
                    printf("* ");
                }else printf("  ");
        }puts("");
    }
}
int borderdown(){//Lower border for UI
    for(int i=0;i<3;i++){
        for(int j=0;j<=20;j++){
                if(i==2||j==0||j==20){
                    printf("* ");
                }else printf("  ");
        }puts("");
    }
}
struct file_var{//Collect mem for file variable?
    char file_username[99][20];
    char file_password[99][20];
    char file_role[99][20];
};

int main(){
    int time;
    struct file_var fv;//Rename file struct into "fv"
    system("cls"); //Clear screen
    //login,register
    int choice;
    borderup();
    printf("  Welcome! Choose your choices\n  1:Login\n  2:Register\n  Please Enter your choice: ");
    scanf("%d",&choice);//Scan for choices
    switch(choice){//Clear screen after the choice is chosen
        case 1: system("cls"); 
        case 2: system("cls"); 
        default: system("cls");
    }
    if(choice!=1&&choice!=2){//If choice is not in the list
        printf("Invalid choice.");
    }
    FILE *file=fopen("cpe100 final.csv","r+"); //Open username,password,role file
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    char line[99]; //for limited the number of user login to the program (Still don't know how to make it unlimited)
    int i=0;
    while(fgets(line, sizeof(line), file)){//Take in information from file
        sscanf(line, "%[^,],%[^,],%[^,]", fv.file_username[i], fv.file_password[i], fv.file_role[i]);
        i++;
    }
    /*for (int j = 1; j < i; j++) {//skip header,printing file's info
        printf("Username: %s\nPassword: %s\nRole: %s\n", 
               fv.file_username[j], 
               fv.file_password[j], 
               fv.file_role[j]);
    }*/
    
   int attemptreg=0;
    while(choice==2&&attemptreg<=3){//register
        int cmp_user=1;
        char username[50],password[50],role[50]="customer",text[50]="Enter Your Username and Password";   
    //username and password must not exceed 20 letters
        if(attemptreg==3){//Using the already-used username more than 3 attempts
            printf("Attempt limit exceed, please try again later.\n");
            printf("Auto-closed in (second)");
            for(int time=5;time>0;time--){
                delay(1);
                printf(" %d",time);
            }
            delay(1);
            system("cls");
            return 0;
            
        }else{ 
            borderup();
            printf("   %s       \n",text);
            printf("   Username : ");
            scanf(" %s",&username);
            if(strlen(username)>20){
                system("cls");
                printf("Username and Password must not exceed 20 letters.");
                return 1;
            }
            printf("   Password : ");
            scanf(" %s",&password);   
            if(strlen(password)>20){
                system("cls");
                printf("Username and Password must not exceed 20 letters.");
                return 1;

            }for(int j=1;j<i;j++){
                cmp_user = strcmp(username, fv.file_username[j]);//check for the username if it's already used or not
                if(cmp_user==0){
                    printf("   Username is already taken.\n  (Please wait for 3 second to re-enter.)\n");
                    attemptreg++;
                    break;
                }}//retry 3 times else reopen program
            if(cmp_user!=0){//if username is unused, regist successful
                fseek(file, 0, SEEK_END);
                fprintf(file, "%s,%s,%s\n", username, password, role);
                printf("   User registered successfully.\n");
                choice=0;
                borderdown();
                break;
                }
            borderdown();
            if(cmp_user==0){//for clearing lower border incase the user's already used.
                delay(3);
                system("cls");
            }}
        //system("cls"); เคลียร์จอหลังจากลงชื่อเสร็จ ตัดไปหน้าซื้อของได้เลย <-- REGISTER จบตรงนี้
    }
    int attemptlog=0;
    while(choice==1&&attemptlog<=3){
        int cmp_user=1,cmp_pass=1;//string compare
        char username[50],password[50],role[50],text[50]="Enter Your Username and Password";   
    //username and password must not exceed 20 letters
        if(attemptlog==3){//If username or password is not correct more than 3 attempts.
            printf("Attempt limit exceed, please try again later.\n");
            printf("Auto-closed in (second)");
            for(int time=5;time>0;time--){
                delay(1);
                printf(" %d",time);
            }
            delay(1);
            system("cls");
            return 0;
            
        }else{
        borderup();
        printf("   %s       \n",text);
        printf("   Username : ");
        scanf(" %s",&username);
        if(strlen(username)>20){
            system("cls");
            printf("Username and Password must not exceed 20 letters.");
            return 1;
        }
        printf("   Password : ");
        scanf(" %s",&password);   
        if(strlen(password)>20){
            system("cls");
            printf("Username and Password must not exceed 20 letters.");
            return 1;

        }for(int j=1;j<i;j++){//check for username and password 
            cmp_user = strcmp(username, fv.file_username[j]);
            cmp_pass = strcmp(password, fv.file_password[j]);
            if(cmp_user==0&&cmp_pass==0){//if there's username and password stored
                printf("   Login successfully.\n");
                choice=0;
                break;
                }//login function <-- จบตรงนี้
            }if(cmp_user!=0||cmp_pass!=0){
                printf("   Wrong username or password.\n  (Please wait for 3 second to re-enter.)\n");
                attemptlog++;
                }//retry 3 times else reopen program
        borderdown();
        if(cmp_user!=0||cmp_pass!=0){//for clearing lower border incase wrong password or username
            delay(3);
            system("cls");
        }}
    }
    free(file);
    fclose(file);
}