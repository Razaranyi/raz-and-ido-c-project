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

/*get file path and return the file dir*/
char *get_dir_path(char *file_path);

/*get dir and file name and return the full path*/
char *concat_dir_and_fname(char *dir, char *fname);

/*grouping all the create files (extern, entry, object) function, run it and create 3 files*/
void create_files(DoublyLinkedList *symbol_table,DoublyLinkedList *encode_line_pair, char * dir);

/*get double linked list of encode line pairs and dir to the file, run over it and for any line enter her hex representation to the object file*/
void create_object_file(DoublyLinkedList *encode_line_pair, char * dir);

/*get double linked list symbol table and dir to the file, run over it and for entry symbols write to ps.ent file*/
void create_entry_file(DoublyLinkedList *symbol_table, char * dir);

/*get double linked list symbol table and dir to the file, run over it and for extern symbols write to ps.ext file*/
void create_extern_file(DoublyLinkedList *symbol_table, char * dir);

#endif