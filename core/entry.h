#ifndef ENTRY_H
#define ENTRY_H
#include "doubly_linked_list.h"
#include "../utils/commons.h"
#include "../utils/boolean.h"

/*Struct to represent an entry instruction to be handled in the second pass*/
typedef struct Entry {
    char* name;
    int index;
}Entry;
/*Allocate memory for new struct*/
Entry* allocate_entry_mem( char* name, int index);

/*free entry*/
int free_entry(Entry* entry);

/*Wrapper for free_entry so it will match free_list signature*/
void free_entry_data(void* data);

#endif
