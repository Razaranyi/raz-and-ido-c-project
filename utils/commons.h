#ifndef COMMON_H
#define COMMON_H
#include "common_types.h"

/* Boolean values: TRUE or FALSE */
typedef enum {
    FALSE,
    TRUE
} Boolean;

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

#endif /* COMMON_H */
