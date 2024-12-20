#ifndef ENTRY_H
#define ENTRY_H
#include "doubly_linked_list.h"
#include "../utils/commons.h"
#include "../utils/boolean.h"


typedef struct Entry {
    char* name;
    int index;
}Entry;
Entry* allocate_entry_mem( char* name, int index);
int free_entry(Entry* entry);
void free_entry_data(void* data);

#endif
