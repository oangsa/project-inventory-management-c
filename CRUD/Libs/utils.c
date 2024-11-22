#include <stdlib.h>
#include <string.h>
#include "utils.h"

char* copyString(char *s, int size) {
    char* s2;
    s2 = (char*)malloc(size);

    strcpy(s2, s);
    return (char*)s2;
}
