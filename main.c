#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/commons.h"
#include "utils/logger.h"
#include "utils/macro_part.h"
#include "core/doubly_linked_list.h"
#include "core/command.h"

int main(int argc, char* argv[]) {
    int i = 0; /*for the big loop*/
    printf("Assembler started.\n");
	if (argc < 2)
	{
		printf("ERROR - There is not a file name \n");
		return 1;
	}
    Macro * Macros = create_macro_table(); /*for the macros*/
    initialize_command_set();


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
		checker = write_without_macro(fname, &Macros);
		free(fname);
		if (checker == FALSE)
		{
			printf("Error in the Macros part - the program stop \n");
			exit(FALSE);
		}
		/*checker = first_reading(fname);*/
	}
	free_macro_table(Macros);
	return 0;
    return 0;
}
