#include "entry.h"


Entry* allocate_entry_mem( char* name, int index){
    Entry *res = malloc(sizeof(Entry));
    res->name = allocate_string(name);
    res->index = index;
    return res;
}

int free_entry(Entry* entry){
    if (entry == NULL){
        return FALSE;
    }
    free(entry->name);
    free(entry);
    return TRUE;
}

void free_entry_data(void* data) {
    Entry* entry = (Entry*)data;
    if (entry != NULL){
        free_entry(entry);
    }
}



