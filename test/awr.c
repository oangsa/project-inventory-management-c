#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct subject {
    char *code;
    float grade;
    float credit;
    struct subject *next;

} subjects;

void adder(int *ctr) {++*ctr;}

float getCredits(subjects *arr) {
    subjects *subTmp;
    float sum = 0;

    for (int i = 1;; i++ ) {
        sum += arr->credit; 

        if (arr->next == NULL) break; 

        subTmp = arr->next;
        arr = subTmp;
    }

    return sum;
}

float calGpax(subjects *arr) {
    subjects *subTmp;
    float sum = 0;
    float credits = 0;

    for (int i = 1;; i++ ) {
        sum += (float) arr->grade * (float) arr->credit;
        credits += arr->credit;

        if (arr->next == NULL) break; 
        
        subTmp = arr->next;
        arr = subTmp;
    }

    return sum / credits;
}

float calGpaxOfSubject(subjects *arr, char *subject) {
    subjects *subTmp;
    float sum = 0;
    float credits = 0;

    for (int i = 1;; i++ ) {
        if (strstr(arr->code, subject)) {
            sum += arr->grade * arr->credit;
            credits += arr->credit;
        }
        
        if (arr->next == NULL) break; 
        
        subTmp = arr->next;
        arr = subTmp;
    }

    return sum / credits;
}

char* copyString(char *s, int size) {
    char* s2;
    s2 = (char*)malloc(size);
 
    strcpy(s2, s);
    return (char*)s2;
}

int main(void) {
    FILE *fptr;
    char *myString;
    char *token;
    int test = 0;

    char code[7];
    float credit;
    float gpa;
    
    subjects *arr = malloc(1 * sizeof(subjects));

    fptr = fopen("hello.txt", "r");

    while(!feof(fptr)) {
        fscanf(fptr, "%6s %f %f", code, &credit, &gpa);

        if(test == 0) {
            arr->code = copyString(code, strlen(code));

            arr->credit = credit;

            arr->grade = gpa;

            arr->next = NULL;

            adder(&test);
        }
        else {
            subjects *tmpArr = malloc(sizeof(subjects));
            
            tmpArr->code = copyString(code, strlen(code));

            tmpArr->credit = credit;

            tmpArr->grade = gpa;

            tmpArr->next = arr;
            arr = tmpArr;
        }
    }

    fclose(fptr);
    // subjects *subTmp;

    // for (int i = 1;; i++ ) {
    //     printf("%d.\n", i);
    //     printf("Subject: %s\n", arr->code);
    //     printf("Grade: %f\n", arr->grade);
    //     printf("Credit: %f\n", arr->credit);
    //     printf("==================\n");
        
    //     if (arr->next == NULL) break;
        
    //     subTmp = arr->next;
    //     arr = subTmp;
    // }

    printf("Gpax MTH: %f\n", calGpaxOfSubject(arr, "MTH"));
    printf("Gpax SCI: %f\n", calGpaxOfSubject(arr, "SCI"));
    printf("Gpax ENG: %f\n", calGpaxOfSubject(arr, "ENG"));
    printf("Gpax: %f\n", calGpax(arr));

    return 0;
}