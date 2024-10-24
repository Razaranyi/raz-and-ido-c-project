#include <string.h>
#include "commons.h"

char* allocate_string(char* string){
    int str_len = strlen(string);
    char* res = malloc(str_len + 1);
    strcpy(res,string);
    return  res;
}

int* allocate_int(int integer){
    int* ptr = malloc(sizeof (int));
    *ptr = integer;
    return ptr;
}
