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

    char subjs[99][7] = {"THA101", "MTH101", "MTH201", "SCI201", "SCI221", "SCI113", "SCI115", "SOC101", "SOC231", "PE101", "PE203", "ART101", "ENG101", "ENG201", "CHI213", "THA102", "MTH102", "MTH202", "SCI261", "SCI202", "SCI222", "SCI241", "SCI284", "SOC102", "SOC232", "PE102", "ART204", "ART102", "CAR101", "ENG102", "CHI214", "THA101", "MTH101", "MTH201", "SCI203", "SCI112", "SCI223", "SCI242", "SCI116", "SOC101", "SOC233", "PE101", "PE246", "ART101", "ENG101", "ENG202", "IS201", "THA102", "MTH102", "MTH202", "SCI261", "SCI204", "SCI224", "SCI243", "SCI283", "SOC102", "SOC234", "PE102", "PE247", "ART102", "CAR101", "ENG102", "IS202", "THA101", "MTH101", "MTH201", "SCI111", "SCI225", "SCI244", "SCI285", "SCI114", "SOC101", "PE101", "PE205", "ART101", "ENG101", "ENG203", "THA102", "MTH102", "MTH202", "SCI205", "SCI245", "SCI286", "SOC102", "PE102", "PE206", "ART102", "ENG102"};
    float credits[99] = {1.0, 1.0, 1.5, 2.0, 1.5, 1.5, 1.0, 1.5, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.5, 1.0, 2.0, 1.5, 1.5, 0.5, 1.5, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.5, 2.0, 1.5, 1.5, 1.5, 0.5, 1.5, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.5, 1.0, 2.0, 1.5, 1.5, 0.5, 1.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.5, 1.5, 1.5, 1.5, 0.5, 1.5, 1.0, 0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.5, 2.0, 1.5, 0.5, 1.0, 0.5, 0.5, 0.5, 1.0};
    float grades[99] = {4, 4, 4, 4, 4, 3.5, 4, 4, 4, 4, 4, 4, 4, 4, 4.0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3.5, 4, 3.5, 4, 3.5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3.5, 3.5, 4, 4, 3.5, 4, 4, 4, 4, 4};

    fptr = fopen("test.txt", "w");

    arr->code = subjs[0];
    arr->grade = grades[0];
    arr->credit = credits[0];
    arr->next = NULL;

    // Insert Node
    for (int i = 1; i < 88; i++) {
        subjects *tmpArr = malloc(sizeof(subjects));
        tmpArr->code = subjs[i];
        tmpArr->grade = (float) grades[i];
        tmpArr->credit = credits[i];
        tmpArr->next = arr;

        arr = tmpArr;
    }

    // printf("%", )

    subjects *subTmp;

    for (;;) {

        fprintf(fptr, "Subject,%s,Credit,%.2f,%.2f", arr->code, arr->credit, arr->grade);

        if (arr->next == NULL) break; 

        fprintf(fptr, "\n");
        
        subTmp = arr->next;
        arr = subTmp;
    }

    fclose(fptr);

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

    return 0;
}