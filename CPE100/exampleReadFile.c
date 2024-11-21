//Suthang Sukrueangkun
//67070503477

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct subject {
    char *name;
    float credit;
    float grade;
} subjects;

char* copyString(char *s, int size) {
    char* s2;
    s2 = (char*)malloc(size);
 
    strcpy(s2, s);
    return (char*)s2;
}

int main(void) {
    
    int i = 0;

    subjects arr[999];

    FILE *myFile;
    FILE *writeFile;
    
    myFile = fopen("Data.csv", "r");
    writeFile = fopen("result.csv", "w");

    if(myFile == NULL) {
        printf("Cannot open file\n");
        return 1;
    }
    
    char name[7];
    float credit;
    float grade;

    while (!feof(myFile)) {
        fscanf(myFile, "%6[^,],%f,%f\n", name, &credit, &grade);
        
        arr[i].name = copyString(name, strlen(name));
        arr[i].credit = credit;
        arr[i].grade = grade;
        
        i+=1;
    }

    fclose(myFile);

    for (int j = 0; j < i; j++) {
        fprintf(writeFile,"%s %.2f %.2f\n", arr[j].name, arr[j].credit, arr[j].grade);
    }

    fclose(writeFile);


    return 0;
}