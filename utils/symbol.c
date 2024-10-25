#include <stdlib.h>
#include "symbol.h"
#include "doubly_linked_list.h"
#include "boolean.h"
#include "commons.h"


Symbol* allocate_sym_mem(char* label, int value, DoublyLinkedList* sym_properties){
    Symbol res = malloc(sizeof(Symbol ));
    res->label = allocate_string(label);
    res->value = value;
    res->sym_properties = sym_properties;
    res->sym_addresses = allocate_node_mem();
}

int is_property(Symbol symbol, int property){

}



int free_symbol(Symbol* symbol){
    free(symbol->label);
    free_list(&symbol->sym_addresses,free);
    free_list(&symbol->sym_properties,free);
    free(symbol);
    return TRUE;
}

