#ifndef RAZ_AND_IDO_C_PROJECT_CREATE_FILES_H
#define RAZ_AND_IDO_C_PROJECT_CREATE_FILES_H
#include "../core/doubly_linked_list.h"
#include "../utils/commons.h"
#include "../utils/line_indexing.h"
#include "../core/instruction.h"
#include "../core/command.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*get double linked list symbol table, run over it and for entry symbols write to ps.ent file*/
void create_entry_file(DoublyLinkedList *symbol_table);

/*get double linked list symbol table, run over it and for external symbols write to ps.ent file*/
void create_extern_file(DoublyLinkedList *symbol_table);

#endif