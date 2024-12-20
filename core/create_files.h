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

/*grouping all the create files (extern, entry, object) function, run it and create 3 files*/
void create_files(DoublyLinkedList *symbol_table,DoublyLinkedList *encode_line_pair, int dc, int ic);

/*get double linked list of encode line pairs, run over it and for any line enter her hex representation to the object file*/
void create_object_file(DoublyLinkedList *encode_line_pair, int dc, int ic);

/*get double linked list symbol table, run over it and for entry symbols write to ps.ent file*/
void create_entry_file(DoublyLinkedList *symbol_table);

/*get double linked list symbol table, run over it and for external symbols write to ps.ext file*/
void create_extern_file(DoublyLinkedList *symbol_table);

#endif