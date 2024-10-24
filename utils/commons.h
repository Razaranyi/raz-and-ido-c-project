#ifndef COMMON_H
#define COMMON_H
#include "common_types.h"


typedef enum {
    FALSE,
    TRUE
} Boolean;

char* allocate_string(char* string);

int* allocate_int(int integer);

int is_string_begin_with_substring(char* string, char* substring);

int is_label(char* string);

int is_string_equal_by_regex(char* string, char* pattern);

int split_string_by_separator(char* string,char* separator, DoublyLinkedList** result_list, int max_splits);

int replace_spaces_with_single_space(char* string);

int remove_leading_and_trailing_whitespaces(char* string,char* result);

int str_substring(char* string, int start_ind, int end_ind, char* result);

#endif
