#include <stdlib.h>
#include "symbol.h"
#include "doubly_linked_list.h"
#include "boolean.h"
#include "commons.h"


Symbol* allocate_sym_mem(char* label, int value, DoublyLinkedList* sym_properties){
    Symbol* res = malloc(sizeof(Symbol));
    res->label = allocate_string(label);
    res->value = value;
    res->sym_properties = sym_properties;
    res->sym_addresses = allocate_node_mem();
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
    return is_property(symbol,ENTRY);
}

int is_external(Symbol symbol){
    return is_property(symbol,EXTERNAL);
}

int free_symbol(Symbol* symbol){
    free(symbol->label);
    free_list(&symbol->sym_addresses,free);
    free_list(&symbol->sym_properties,free);
    free(symbol);
    return TRUE;
}

