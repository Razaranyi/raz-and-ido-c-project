#include <string.h>
#include <regex.h>
#include <ctype.h>
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

int is_string_begin_with_substring(char* string, char* substring){
    return strstr(string,substring) == string;
}

int is_label( char* line) {
    /* Pattern to match a label: a valid label name ending with ':' */
    const char* pattern = "^[A-Za-z][A-Za-z0-9]*:$";
    return is_string_equal_by_regex(line, pattern);
}


int is_string_equal_by_regex(char* string, char* pattern) {
    regex_t reg;
    int res;

    /* Compile the regular expression */
    if (regcomp(&reg, pattern, REG_EXTENDED) != 0) {
        return 0;  /* Return 0 if regex compilation fails */
    }

    /* Execute the regular expression */
    res = regexec(&reg, string, 0, NULL, 0);

    regfree(&reg);

    return res != REG_NOMATCH;
}

int str_substring(const char* string, int start_ind, int end_ind, char* result) {
    int string_length = strlen(string);

    if (end_ind == -1) {
        end_ind = string_length;
    }

    /* Check for valid indices */
    if (string_length != 0 && (start_ind < 0 || start_ind >= string_length || end_ind < start_ind || end_ind > string_length)) {
        return FALSE;
    }

    /* Copy the substring to result */
    strncpy(result, string + start_ind, end_ind - start_ind);
    result[end_ind - start_ind] = '\0';

    return TRUE;
}

int remove_trailing_whitespaces(char* string, char* result) {
    int start_ind;
    int end_ind;
    int i, j;

    for (start_ind = 0; isspace((unsigned char)string[start_ind]); start_ind++) {
        /* Keep looping until not space */
    }

    for (end_ind = strlen(string) - 1; end_ind >= start_ind && isspace((unsigned char)string[end_ind]); end_ind--) {
        /* Keep looping until not space */
    }
    return str_substring(string,start_ind,end_ind,result);
}