#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/commons.h"
#include "utils/logger.h"
#include "exec/macro_parsing.h"
#include "utils/line_indexing.h"
#include "core/doubly_linked_list.h"

int main(int argc, char* argv[]) {
    int i = 0; /*for the big loop*/
    DoublyLinkedList *macro_list = create_macro_table(); /*for the macros*/
    DoublyLinkedList *line_list = create_line_table(); /*for the lines*/
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
		checker = write_without_macro(fname, macro_list, line_list);

		if (checker == FALSE)
		{
			printf("Error in the Macros part - the program stop \n");
            free_macro_table(macro_list);
            free_line_table(line_list);
			exit(FALSE);
		}
		/*checker = first_reading(fname);*/
	}
	print_lines(line_list);
    free_macro_table(macro_list);
    free_line_table(line_list);

    return 0;
}
