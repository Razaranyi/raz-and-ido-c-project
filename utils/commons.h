#ifndef COMMON_H
#define COMMON_H
#include "doubly_linked_list.h"

typedef enum {
    FALSE,
    TRUE
} Boolean;

/*Returns a pointer to a new string*/
char* allocate_string(char* string);

/*Cut a string by indexes*/

int str_cut(char* string, int start_index, int end_index, char* result);

int str_split_by_separator(char* string, DoublyLinkedList* result, int max_splits);



#endif
