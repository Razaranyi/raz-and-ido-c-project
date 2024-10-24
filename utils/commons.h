#ifndef COMMON_H
#define COMMON_H
#include "doubly_linked_list.h"


typedef enum {
    FALSE,
    TRUE
} Boolean;

char* allocate_string(char* string);

int* allocate_int(int integer);

int is_string_begin_with_substring(char* string, char* substring);

int is_label(char* string);

int split_string_by_separator(char* string,char* separator, DoublyLinkedList** result_list, int max_splits);

int replace_spaces_with_single_space(char* string);





#endif
