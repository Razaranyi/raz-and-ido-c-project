/* test_commons.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/doubly_linked_list.h"
#include "../utils/commons.h"

/* Function to free data stored in the list nodes */
void free_node_data(void* data) {
    free(data);
}

int main() {
    /* Declare all variables at the beginning */
    char* original_string;
    char* copied_string;
    int original_int;
    int* copied_int;
    char* string;
    char* substring;
    int begins_with;
    char* labels[] = {"Label1:", "1Label:", "Label:", "Label: some text", NULL};
    int i;  /* Loop counter declared outside the for loop */
    char* test_string;
    char* pattern;
    int regex_match;
    char substring_result[50];
    int substring_success;
    char string_with_spaces[] = "   \t\n Hello, World! \t\n   ";
    char trimmed_string[50];
    int trim_success;
    char* test_string_no_max_splits;
    DoublyLinkedList* result_list = NULL;
    DoublyLinkedList* current;
    int part_num;
    char* part;
    char* test_string_max_splits;
    int max_splits_values[] = {1, 2, 3, -1};
    int max_splits;
    DoublyLinkedList* result_list_max_splits = NULL;

    /* Test allocate_string */
    printf("Testing allocate_string:\n");
    original_string = "Hello, World!";
    copied_string = allocate_string(original_string);
    printf("Original string: '%s'\n", original_string);
    printf("Copied string: '%s'\n\n", copied_string);

    /* Test allocate_int */
    printf("Testing allocate_int:\n");
    original_int = 42;
    copied_int = allocate_int(original_int);
    printf("Original int: %d\n", original_int);
    printf("Copied int: %d\n\n", *copied_int);

    /* Test is_string_begin_with_substring */
    printf("Testing is_string_begin_with_substring:\n");
    string = "Hello, World!";
    substring = "Hello";
    begins_with = is_string_begin_with_substring(string, substring);
    printf("Does '%s' begin with '%s'? %s\n\n", string, substring, begins_with ? "Yes" : "No");

    /* Test is_label */
    printf("Testing is_label:\n");
    for (i = 0; labels[i] != NULL; i++) {
        printf("Is '%s' a label? %s\n", labels[i], is_label(labels[i]) ? "Yes" : "No");
    }
    printf("\n");

    /* Test is_string_equal_by_regex */
    printf("Testing is_string_equal_by_regex:\n");
    test_string = "abc123";
    pattern = "^[a-zA-Z]+[0-9]+$";
    regex_match = is_string_equal_by_regex(test_string, pattern);
    printf("Does '%s' match pattern '%s'? %s\n\n", test_string, pattern, regex_match ? "Yes" : "No");



    /* Test str_substring */
    printf("Testing str_substring:\n");
    substring_success = str_substring(string, 7, -1, substring_result);
    if (substring_success) {
        printf("Substring of '%s' from index 7 to end: '%s'\n\n", string, substring_result);
    } else {
        printf("Failed to get substring.\n\n");
    }

    /* Test remove_leading_and_trailing_whitespaces */
    printf("Testing remove_leading_and_trailing_whitespaces:\n");
    trim_success = remove_leading_and_trailing_whitespaces(string_with_spaces, trimmed_string);
    if (trim_success) {
        printf("Original string with spaces: '%s'\n", string_with_spaces);
        printf("Trimmed string: '%s'\n\n", trimmed_string);
    } else {
        printf("Failed to trim string.\n\n");
    }

    /* Test split_string_by_separator without max_splits */
    printf("Testing split_string_by_separator without max_splits:\n");
    test_string_no_max_splits = "apple,banana,cherry";
    split_string_by_separator(test_string_no_max_splits, ",", &result_list, -1);

    /* Print the split results */
    current = get_list_head(result_list);
    part_num = 1;
    while (current != NULL) {
        part = (char*)current->data;
        printf("Part %d: '%s'\n", part_num, part);
        current = current->next;
        part_num++;
    }
    free_list(&result_list, free_node_data);
    printf("\n");

    /* Test split_string_by_separator with max_splits */
    printf("Testing split_string_by_separator with max_splits:\n");
    test_string_max_splits = "one,two,three,four,five";
    for (i = 0; i < (int)(sizeof(max_splits_values)/sizeof(int)); i++) {
        max_splits = max_splits_values[i];
        result_list_max_splits = NULL;
        split_string_by_separator(test_string_max_splits, ",", &result_list_max_splits, max_splits);

        /* Print the split results */
        printf("Test with max_splits = %d:\n", max_splits);
        current = get_list_head(result_list_max_splits);
        part_num = 1;
        while (current != NULL) {
            part = (char*)current->data;
            printf("Part %d: '%s'\n", part_num, part);
            current = current->next;
            part_num++;
        }
        free_list(&result_list_max_splits, free_node_data);
        printf("\n");
    }

    /* Clean up */
    free(copied_string);
    free(copied_int);

    return 0;
}
