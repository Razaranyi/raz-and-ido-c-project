#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <ctype.h>
#include "commons.h"
#include "doubly_linked_list.h"


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
    char* pattern = "^[a-zA-Z][a-zA-Z0-9]*:.*";
    return is_string_equal_by_regex(line, pattern);
}


int is_string_equal_by_regex(char* string, char* pattern) {
    regex_t reg;
    int res;

    /* compile the regular expression */
    if (regcomp(&reg, pattern, REG_EXTENDED | REG_ICASE) != 0) {
        return 0;  /*regex compilation fails */
    }

    /* exec the regular expression */
    res = regexec(&reg, string, 0, NULL, 0);

    regfree(&reg);

    return res != REG_NOMATCH;
}

int str_substring(char* string, int start_ind, int end_ind, char* result) {
    int string_length = strlen(string);

    if (end_ind == -1) {
        end_ind = string_length;
    }

    /* check for valid indices */
    if (string_length != 0 && (start_ind < 0 || start_ind >= string_length || end_ind < start_ind || end_ind > string_length)) {
        return FALSE;
    }

    /* copy the substring to result */
    strncpy(result, string + start_ind, end_ind - start_ind);
    result[end_ind - start_ind] = '\0';

    return TRUE;
}

int remove_leading_and_trailing_whitespaces(char* string, char* result) {
    int start_ind;
    int end_ind;

    for (start_ind = 0; isspace((unsigned char)string[start_ind]); start_ind++) {
        /* Keep looping until not space */
    }

    for (end_ind = strlen(string) - 1; end_ind >= start_ind && isspace((unsigned char)string[end_ind]); end_ind--) {
        /* Keep looping until not space */
    }
    return str_substring(string,start_ind,end_ind,result);
}

char* skip_separators(char* string, char* separator) {
    while (*string && strchr(separator, *string)) {
        string++;
    }
    return string;
}

char* get_last_separator(char* string, char* separator) {
    int separator_length = strlen(separator);
    char* current = string + strlen(string) - 1;

    while (strstr(current - separator_length, separator) == current && current - separator_length >= current) {
        current -= separator_length;
    }

    return current + 1;
}

int split_string_by_separator(char* string,char* separator, DoublyLinkedList** result_list, int max_splits){
    char* current;
    char item[4096];
    int number_of_splits =0;

    DoublyLinkedList* items_list = allocate_node_mem();

    do {
        number_of_splits++;
        string = skip_separators(string,separator);
        current = (strstr(string,separator));

        if(current == NULL){
            strcpy(item,string);
        } else{
            str_substring(string,0,current - string,item);
        }

        add_to_list(items_list, allocate_string(item));
        string = current;
    } while ((max_splits == -1 || number_of_splits < max_splits) && current != NULL && strlen(string) > 0);

    if(string != NULL && number_of_splits == max_splits){
        string = skip_separators(string,separator);
        current = get_last_separator(string,separator);

        if(string != current){
            str_substring(string, 0, current - string,item);
            add_to_list(items_list, allocate_string(item));
        }
    }

    *result_list = items_list;
    return 0;
}