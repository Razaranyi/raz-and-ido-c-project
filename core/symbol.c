#include <stdlib.h>
#include <string.h>
#include "symbol.h"
#include "doubly_linked_list.h"
#include "../utils/boolean.h"
#include "../utils/commons.h"


Symbol* allocate_sym_mem(char* label, unsigned address, DoublyLinkedList* sym_properties){
    Symbol* res = malloc(sizeof(Symbol));
    res->label = allocate_string(label);
    res->address = address;
    res->sym_properties = sym_properties;
    return res;
}

int is_property(Symbol symbol, SymbolProperty property) {
    int current;
    int* data;
    DoublyLinkedList* aliased_list = symbol.sym_properties;

    while (aliased_list != NULL && !is_list_empty(aliased_list)) {
        data = aliased_list->data;
        current = *data;
        if (current == property) {
            return TRUE;
        }
        aliased_list = aliased_list->next;
    }
    return FALSE;
}

int is_entry(Symbol symbol){
    return is_property(symbol, ENTRY_PROPERTY);
}

int is_external(Symbol symbol){
    return is_property(symbol, EXTERNAL_PROPERTY);
}

/* Adds a symbol to the symbol table */
int add_symbol(DoublyLinkedList* symbol_table, char* label, unsigned long address, SymbolProperty property, int index) {
    DoublyLinkedList* current;
    DoublyLinkedList* properties;
    Symbol* new_symbol;
    if (symbol_table == NULL || label == NULL) {
        return FALSE;
    }

    /* Check if the label already exists in the symbol table */
    current = get_list_head(symbol_table);
    while (current != NULL) {
        Symbol* existing_symbol = (Symbol*)current->data;
        if (strcmp(existing_symbol->label, label) == 0) {
            errorf(index, "Duplicate label found: '%s'", label);
            return FALSE;
        }
        current = current->next;
    }

    /* Create a new symbol */
    properties = allocate_node_mem();
    add_to_list(properties, allocate_int(property));
    new_symbol = allocate_sym_mem(label, address, properties);

    /* Add the new symbol to the table */
    if (add_to_list(symbol_table, new_symbol) != TRUE) {
        free_symbol(new_symbol); /* Free allocated memory on failure */
        return FALSE;
    }

    return TRUE;
}

int free_symbol(Symbol* symbol){
    if (symbol == NULL){
        return FALSE
    }
    free(symbol->label);
    free_list(&symbol->sym_properties,free);
    free(symbol);
    return TRUE;
}

