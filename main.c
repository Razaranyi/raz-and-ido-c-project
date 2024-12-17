#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/commons.h"
#include "utils/logger.h"
#include "exec/macro_parsing.h"
#include "utils/line_indexing.h"
#include "core/address_encoded_pair.h"
#include "core/doubly_linked_list.h"
#include "exec/compiler.h"
#include "symbol.h"
#include "core/create_files.h"

int main(int argc, char* argv[]) {
    int i = 0; /*for the big loop*/
    DoublyLinkedList *macro_list = allocate_node_mem(); /*for the macros*/
    /*TODO: add file name to the indexed line for logging purposes*/
    DoublyLinkedList *line_list = allocate_node_mem();/*for the lines*/
    DoublyLinkedList *symbol_table = allocate_node_mem();
    DoublyLinkedList *address_encoded_line_pair = allocate_node_mem();
    initialize_command_set();

    printf("Assembler started...\n");
	if (argc < 2)
	{
		printf("ERROR - There is not a file name \n");
		return 1;
	}
	
	for(i = 1; i < argc; i++)
	{
		int checker = 0; /*checks if we get problem while reading and parse the file if 1 its all good if 0 its bad*/
		char* fname = malloc(strlen(argv[i]) +1);/*create memory space for fname*/
		if (fname == NULL) /*looks for allocation problems*/
		{
			printf("Memory allocation failed. \n");
			return -1; 
		}
		strcpy(fname, argv[i]); /*enter the user input to fname*/
		/*write without macros, and get the line list index and macros list*/
		checker = parse_macro(fname, macro_list, line_list);
		if (checker == FALSE)
		{
			fatal("Error in the Macros part - the program stop",-1);
            free_macro_table(macro_list);
            free_line_table(line_list);
			exit(FALSE);
		}

        first_pass(line_list,symbol_table,address_encoded_line_pair);
        print_address_encode_list(address_encoded_line_pair);


		/*checker = first_reading(fname);*/
	}
	create_entry_file(symbol_table);
    free_macro_table(macro_list);
    free_line_table(line_list);

    return 0;
}
