#ifndef __MACRO_PART__H__
#define __MACRO_PART__H__
#include "../utils/line_indexing.h"
#include "../core/doubly_linked_list.h"



/*struct for macro table*/
typedef struct {
    char *macroName;
    char *data;
    int macroIndex;
} Macro;



/*read the assembley file and parse the macro parts return 0 if theres a problems while reading the macros*/
int parse_macro(char *fname, DoublyLinkedList *macro_list, DoublyLinkedList *line_list);


/*get all the macros from the file return int to check if get errors in the macro names, if 0 unvalid if 1 valid*/
int get_macros(FILE *fp, DoublyLinkedList *macro_list);


/*add Macro to the Macro table*/
void add_macro(DoublyLinkedList *macro_list,char *macroName, char *data, int linecounter);

/*check if name is in the macro table if yes return macro data, else NULL*/
char* in_macro_table(char *name, DoublyLinkedList *macro_list);

/*check if name is in the macro table if yes return macro index, else -1*/
int get_index_macro_table (char *name, DoublyLinkedList *macro_list);

/*run over the macros table and free it*/
void free_macro_table(DoublyLinkedList *macro_list);
#endif  /*!__MACRO_PART__H__*/

void print_macros(DoublyLinkedList *macro_list);
