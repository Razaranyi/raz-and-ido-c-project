
#include "symbol.h"


Symbol* allocate_sym_mem(char* label, unsigned address, unsigned long value, DoublyLinkedList* sym_properties){
    Symbol* res = malloc(sizeof(Symbol));
    res->label = allocate_string(label);
    res->address = address;
    res->value = value;
    res->sym_properties = sym_properties;
    res->external_usages = NULL; /*will be added on second pass if needed*/
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
int add_symbol(DoublyLinkedList* symbol_table, char* label, unsigned long address, unsigned long value, SymbolProperty property, int index) {
    DoublyLinkedList* current;
    DoublyLinkedList* properties;
    Symbol* new_symbol;

    if (strcmp(label,"") == 0){
        return TRUE;
    }

    if (symbol_table == NULL || label == NULL) {
        return FALSE;
    }

    /* Check if the label already exists in the symbol table */
    debugf(index, "Adding symbol: label='%s', address=%lu, value:%lu property=%d", label, address,value, property);
    current = get_list_head(symbol_table);
    while (current != NULL) {
        Symbol* existing_symbol = (Symbol*)current->data;
        if (existing_symbol != NULL ) {
            if (strcmp(existing_symbol->label, label) == 0) {
                errorf(index, "Duplicate label found: '%s'", label);
                return FALSE;
            }
        }


        current = current->next;
    }

    /* Create a new symbol */
    properties = allocate_node_mem();
    add_to_list(properties, allocate_int(property));
    new_symbol = allocate_sym_mem(label, address, value, properties);

    /* Add the new symbol to the table */
    if (add_to_list(symbol_table, new_symbol) != TRUE) {
        free_symbol(new_symbol); /* Free allocated memory on failure */
        return FALSE;
    }

    return TRUE;
}

int add_external_usage(Symbol* symbol, unsigned long usage_address) {
    unsigned long* usage_ptr;
    if (symbol == NULL) {
        return FALSE;
    }

    if (symbol->external_usages == NULL) {
        symbol->external_usages = allocate_node_mem();
        if (symbol->external_usages == NULL) {
            errorf(-1, "Failed to allocate memory for external_usages list");
            return FALSE;
        }
    }

    usage_ptr = (unsigned long*)malloc(sizeof(unsigned long));
    if (!usage_ptr) {
        errorf(-1, "Failed to allocate memory for external usage address");
        return FALSE;
    }
    *usage_ptr = usage_address;

    add_to_list(symbol->external_usages, usage_ptr);

    return TRUE;
}


int free_symbol(Symbol* symbol){
    if (symbol == NULL){
        return FALSE;
    }
    free(symbol->label);
    free_list(&symbol->sym_properties,free);
    if (symbol->external_usages != NULL){
        free_list(&symbol->external_usages,free);
    }
    free(symbol);
    return TRUE;
}

void free_symbol_data(void* data) {
    Symbol* symbol = (Symbol*)data;
    if (symbol != NULL) {
        free_symbol(symbol);
    }
}
int symbols_table_get_symbol(DoublyLinkedList *symbol_table, char *name, Symbol **symbol) {
    DoublyLinkedList *current = get_list_head(symbol_table);

    while (current != NULL) {
        Symbol *current_symbol = (Symbol *)current->data;
        if (strcmp(current_symbol->label, name) == 0) {
            *symbol = current_symbol;
            return TRUE;
        }
        current = current->next;
    }

    return FALSE;
}

int mark_symbol_as_entry(DoublyLinkedList *symbol_table, char *symbol_name, int line_index) {
    DoublyLinkedList *current = get_list_head(symbol_table);

    while (current != NULL) {
        Symbol *symbol = (Symbol *)current->data;

        if (strcmp(symbol->label, symbol_name) == 0) {
            /* Check if ENTRY_PROPERTY already present */
            DoublyLinkedList *prop_node = get_list_head(symbol->sym_properties);
            while (prop_node != NULL) {
                SymbolProperty *sym_prop = (SymbolProperty *)prop_node->data;
                if (*sym_prop == ENTRY_PROPERTY) {
                    /* Already entry */
                    return TRUE;
                }
                prop_node = prop_node->next;
            }

            /* Add ENTRY_PROPERTY if not found */
            add_to_list(symbol->sym_properties, allocate_int(ENTRY_PROPERTY));
            return TRUE;
        }
        current = current->next;
    }

    errorf(line_index, "Symbol '%s' not found in the symbol table to mark as entry", symbol_name);
    return FALSE;
}

void mark_entries(DoublyLinkedList *entry_list, DoublyLinkedList *symbol_table, int *error_found) {
    DoublyLinkedList *current = get_list_head(entry_list);

    while (current != NULL) {
        Entry *entry = (Entry *)current->data;
        if (entry!= NULL && !mark_symbol_as_entry(symbol_table, entry->name, entry->index)) {
            *error_found = TRUE;
        }
        current = current->next;
    }
}


