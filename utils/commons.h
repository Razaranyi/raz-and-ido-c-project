#ifndef COMMON_H
#define COMMON_H
#include "../core/doubly_linked_list.h"
#include "boolean.h"
#define INSTRUCTIONS_LEN 4
#define LEN_LINE 80
#define INSTRUCTIONS {".data", ".string", ".entry", ".extern"}
#define REGS {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"}
#define REGS_LEN 8


/* Allocates memory for a copy of the string */
char* allocate_string(char* string);
/* Usage example:
   char* copy = allocate_string("Hello");
*/

/* Allocates memory for an integer */
int* allocate_int(int integer);
/* Usage example:
   int* num = allocate_int(5);
*/



/* Checks if a string starts with a given substring */
int is_string_begin_with_substring(char* string, char* substring);
/* Usage example:
   is_string_begin_with_substring("Hello", "He"); // Returns TRUE
*/

/* Checks if a string matches the label format (e.g., "LABEL:") */
int is_label(char* string);
/* Usage example:
   is_label("LABEL: something else"); // Returns TRUE
   is_label("123Label: something else"); // Returns FALSE
   is_label("La123bel: something else"); // Returns TRUE
*/

/* Matches a string against a regular expression pattern */
int is_string_equal_by_regex(char* string, char* pattern);
/* Usage example:
   is_string_equal_by_regex("abc123", "^[a-z]+\\d+$"); // Returns TRUE if pattern matches
*/

/* Splits a string by separator, storing results in a linked list */
int split_string_by_separator(char* string, char* separator, DoublyLinkedList** result_list, int max_splits);
/* Usage example:
   DoublyLinkedList* list;
   split_string_by_separator("key1=value1;key2=value2", ";", &list, -1);
   // Splits the string into "key1=value1" and "key2=value2"
*/

/* Replaces multiple spaces with a single space */
int replace_spaces_with_single_space(char* string);
/* Usage example:
   replace_spaces_with_single_space("Hello   World"); // Changes it to "Hello World"
*/

/* Removes leading and trailing spaces */
int remove_leading_and_trailing_whitespaces(char* string, char* result);
/* Usage example:
   char result[100];
   remove_leading_and_trailing_whitespaces("  Hello  ", result); // result = "Hello"
*/

/* Extracts a substring from start to end index */
int str_substring(char* string, int start_ind, int end_ind, char* result);
/* Usage example:
   char result[100];
   str_substring("Hello", 0, 3, result); // result = "Hel"
*/
/*adds .as to the file names we get from the user in the CLI*/
char * add_as( char * fname);

/*adds .am to the file names we get from the user in the CLI*/
char * add_am( char * fname);

/*get input and remove the whitespaces in word
return void, change the input*/
void  cut_spaces(char * name);

/*cut out spaces from the begging of string
return void, change the input*/
void cut_spaces_start(char * input);


/*cut out vhars from the begging of string till "":"
return void, change the input*/
void cut_two_dots_start(char * input);

/*verify that the malloc works*/
void check_malloc(void * p);


/*get word, checks if word is instruction
return int as boolean*/
int check_if_instruction(char *word);

/*get word, checks if word is registar
return int as boolean*/
int check_if_registar(char *word);

/*get word, list of words and list of words size , 
return bollean int for true or false*/
int is_word_in_list(char *word, char *word_list[], int list_size);

#endif
