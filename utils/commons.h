#ifndef COMMON_H
#define COMMON_H
#include "../core/doubly_linked_list.h"
#include "boolean.h"
#define LEN_LINE 80
#define ADDRESS_LEN 7

/*convert address from usigned long to char * and add 0 in the begging for the files format*/
char * fix_address(unsigned long address);


/* Allocates memory for a copy of the string */
char* allocate_string(char* string);


/* Allocates memory for an integer */
int* allocate_int(int integer);


int is_all_whitespace(const char *str);

/* Checks if a string starts with a given substring */
int is_string_begin_with_substring(char* string, char* substring);


/* Checks if a string matches the label format (e.g., "LABEL:") */
int is_label(char* string);


/* Matches a string against a regular expression pattern */
int is_string_equal_by_regex(char* string, char* pattern);

/* Splits a string by separator, storing results in a linked list */
int split_string_by_separator(char* string, char* separator, DoublyLinkedList** result_list, int max_splits);


/* Removes leading and trailing spaces */
int remove_leading_and_trailing_whitespaces(char* string, char* result);


/*get filename and suffix and add the suffix to the file to write fullpath*/
char* add_end_to_filename(const char* filename, const char* suffix);

/*get input and remove the whitespaces in word
return void, change the input*/
void  cut_spaces(char * name);

/*cut out spaces from the begging of string
return void, change the input*/
void cut_spaces_start(char * input);


/*cut out vhars from the begging of string till "":"
return void, change the input*/
void cut_two_dots_start(char * input);



/*get word, list of words and list of words size , 
return bollean int for true or false*/
int is_word_in_list(char *word, char *word_list[], int list_size);

#endif
