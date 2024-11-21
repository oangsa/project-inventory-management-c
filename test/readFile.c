#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct subject {
    char *code;
    float grade;
    float credit;
    struct subject *next;

} subjects;

int main(void) {
    FILE *fptr;
    subjects *arr = malloc(1 * sizeof(subjects));

    char *myString;

    fptr = fopen("hello.txt", "r");

    if(fptr != NULL) {
        while(fgets(myString, 100, fptr)) {
            printf("%s", myString);
        }
    } 
    else {
        printf("Not able to open the file.");
    }

    // printf("%", ) 

    subjects *subTmp;


    fclose(fptr);

    return 0;
}