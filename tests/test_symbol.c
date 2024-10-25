#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../core/symbol.h"
#include "../core/doubly_linked_list.h"
#include "commons.h"
#include "boolean.h"

/* Function to free data stored in the list nodes */
void free_node_data(void* data) {
    free(data);
}

void test_symbol_functions() {
    /* Variable declarations at the beginning */
    char* test_label = "testLabel";
    int test_value = 42;
    int entry_property = ENTRY;
    int external_property = EXTERNAL;
    DoublyLinkedList* properties;
    Symbol* symbol;

    /* Initialize a list for properties with ENTRY and EXTERNAL values */
    properties = allocate_node_mem();
    add_to_list(properties, allocate_int(entry_property));
    add_to_list(properties, allocate_int(external_property));

    /* Allocate memory for the symbol */
    symbol = allocate_sym_mem(test_label, test_value, properties);

    /* Assertions to verify correct allocation and values */
    assert(symbol != NULL);
    assert(symbol->label != NULL);
    assert(strcmp(symbol->label, test_label) == 0);  /* Verify label */
    assert(symbol->value == test_value);  /* Verify value */
    assert(symbol->sym_properties == properties);  /* Check properties list */
    assert(symbol->sym_addresses != NULL);  /* Check sym_addresses list initialization */

    /* Check if the symbol is correctly recognized as entry and external */
    assert(is_entry(*symbol) == TRUE);  /* Check for ENTRY property */
    assert(is_external(*symbol) == TRUE);  /* Check for EXTERNAL property */

    /* Output results */
    printf("Symbol allocation and property test passed:\n");
    printf("Label: %s\n", symbol->label);
    printf("Value: %d\n", symbol->value);
    printf("Contains ENTRY: %s\n", is_entry(*symbol) ? "Yes" : "No");
    printf("Contains EXTERNAL: %s\n", is_external(*symbol) ? "Yes" : "No");
    printf("Addresses list initialized: %s\n", !is_list_empty(symbol->sym_addresses) ? "Yes" : "No");

    /* Free allocated memory */
    free(symbol->label);
    free_list(&symbol->sym_properties, free_node_data);
    free_list(&symbol->sym_addresses, free_node_data);
    free(symbol);
}

int main() {
    test_symbol_functions();
    return 0;
}
