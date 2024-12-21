#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/globals.h"
#include "utils/commons.h"
#include "utils/logger.h"
#include "exec/macro_parsing.h"
#include "utils/line_indexing.h"
#include "core/address_encoded_pair.h"
#include "core/doubly_linked_list.h"
#include "exec/compiler.h"
#include "core/create_files.h"


void got_error(char* fname, char* stage_name);
void free_all( DoublyLinkedList *symbol_table,
          DoublyLinkedList *address_encoded_line_pair,
          DoublyLinkedList *macro_list,
          DoublyLinkedList *line_list,
          DoublyLinkedList *entry_list,
          char* fname);

int main(int argc, char* argv[]) {
    int i = 0; /*for the big loop*/
    initialize_command_set();
    set_log_level(INFO);
    infof(-1,"Assembler started...");
	if (argc < 2)
	{
		printf("ERROR - There is no file name \n");
		return 1;
	}
	
	for(i = 1; i < argc; i++)
	{
        DoublyLinkedList *symbol_table = allocate_node_mem();
        DoublyLinkedList *address_encoded_line_pair = allocate_node_mem();
        DoublyLinkedList *macro_list = allocate_node_mem();
        DoublyLinkedList *line_list = allocate_node_mem();
		DoublyLinkedList *entry_list = allocate_node_mem();

        int checker = 0; /*checks if we get problem while reading and parse the file if 1 its all good if 0 its bad*/
		char* fname = malloc(strlen(argv[i]) +1);/*create memory space for fname*/
		if (fname == NULL)
		{
			printf("Memory allocation failed. \n");
            exit(-1);
		}
        infof(-1,"Start processing %s",fname);
		strcpy(fname, argv[i]); /*enter the user input to fname*/
		/*write without macros, and get the line list index and macros list*/
		checker = parse_macro(fname, macro_list, line_list);
		if (!checker)
		{
            got_error("Macro parsing", fname);
            free_all(symbol_table,address_encoded_line_pair,macro_list,line_list,entry_list,fname);
        }

        checker = first_pass(line_list,symbol_table,address_encoded_line_pair, entry_list);

        if (!checker){
            got_error("Compile - first pass", fname);
            free_all(symbol_table,address_encoded_line_pair,macro_list,line_list,entry_list,fname);

        }

        checker = second_pass(symbol_table,address_encoded_line_pair,entry_list);

        if (!checker){
            got_error("Compile - Second pass", fname);
            free_all(symbol_table,address_encoded_line_pair,macro_list,line_list,entry_list,fname);
            exit(FALSE);
        }

        debugf(-1,"Final IC: %lu, Final DC: %lu\n", final_IC, final_DC);
		create_files(symbol_table, address_encoded_line_pair);
        infof(-1,"Finished processing %s", fname);
        free_all(symbol_table,address_encoded_line_pair,macro_list,line_list,entry_list,fname);
    }

    free_command_set();
    return 0;
}

void got_error( char* stage_name, char* fname){
    fatalf(-1,"File: %s.as got error in %s. Check logs for details",fname,stage_name);
}

void free_all( DoublyLinkedList *symbol_table,
DoublyLinkedList *address_encoded_line_pair,
DoublyLinkedList *macro_list,
DoublyLinkedList *line_list,
DoublyLinkedList *entry_list,
char* fname){
    free_macro_table(macro_list);
    free_list(&symbol_table, free_symbol_data);
    free_list(&address_encoded_line_pair, free_address_encoded_pair);
    free_list(&entry_list,free_entry_data);

    free_line_table(line_list);
    free(fname);
}
