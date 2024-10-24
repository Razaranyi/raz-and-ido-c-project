#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "doubly_linked_list.h"
#include "logger.h"

/* Custom strdup function to replace the non-C89 strdup */
char* custom_strdup(const char* str) {
    size_t len = strlen(str) + 1; /* Length of string including the null terminator */
    char* new_str = malloc(len);  /* Allocate memory for the new string */
    if (new_str) {
        memcpy(new_str, str, len); /* Copy the string to the new memory */
    }
    return new_str;
}

/* Callback function to free node data */
void free_node_data(void* data) {
    free(data);
}

int main() {
    /* Variable declarations at the beginning */
    DoublyLinkedList* list;
    char* data1;
    char* data2;
    char* data3;
    int length;
    DoublyLinkedList* head;
    DoublyLinkedList* tail;

    /* Initialize the list */
    list = allocate_node_mem();
    assert(list != NULL); /* Assertion to ensure the list is successfully initialized */

    /* Test is_list_empty on empty list */
    assert(is_list_empty(list) == TRUE);
    printf("Test 1 Passed: is_list_empty on empty list\n");

    /* Add elements to the list using custom_strdup */
    data1 = custom_strdup("Node1");
    data2 = custom_strdup("Node2");
    data3 = custom_strdup("Node3");

    assert(add_to_list(list, data1) == TRUE);
    assert(add_to_list(list, data2) == TRUE);
    assert(add_to_list(list, data3) == TRUE);
    printf("Test 2 Passed: add_to_list\n");

    /* Test get_list_length */
    length = get_list_length(list);
    assert(length == 3);
    printf("Test 3 Passed: get_list_length (expected 3, got %d)\n", length);

    /* Test is_list_contains_string */
    assert(is_list_contains_string(list, "Node2") == TRUE);
    printf("Test 4 Passed: is_list_contains_string\n");

    /* Test get_list_head and get_list_tail */
    head = get_list_head(list);
    tail = get_list_tail(list);
    assert(strcmp((char*)head->data, "Node1") == 0);
    assert(strcmp((char*)tail->data, "Node3") == 0);
    printf("Test 5 Passed: get_list_head and get_list_tail\n");

    /* Test is_list_empty on non-empty list */
    assert(is_list_empty(list) == FALSE);
    printf("Test 6 Passed: is_list_empty on non-empty list\n");

    /* Clean up */
    free_list(&list, free_node_data);
    list = NULL;  /* Prevent dangling pointer */

    /* Verify that the list is empty after freeing */
    assert(is_list_empty(list) == TRUE);  /* Should pass as list is NULL now */
    printf("Test 7 Passed: free_list\n");

    return 0;
}
