#ifndef __MACRO_PART__H__
#define __MACRO_PART__H__
#include "line_part.h"



/*struct for macro table*/
typedef struct Macro_t{
	char * macroName;
	char * data;
	struct Macro_t * next;
} Macro;



/*read the assembley file and parse the macro parts return 0 if theres a problems while reading the macros*/
int write_without_macro(char *fname, Macro ** Macros, Line ** Lines);


/*get all the macros from the file return int to check if get errors in the macro names, if 0 unvalid if 1 valid*/
int getmacros(FILE * fp, Macro ** Macros);


/*create new macro table for the macro section*/
Macro * create_macro_table();


/*add Macro to the Macro table*/
Macro * add_macro(Macro * head);

/*check if name is in the macro table if yes return macro data, else "0"*/
char * in_macro_table(char * name, Macro * Macros);

/*run over the macros table and free it*/
void free_macro_table(Macro * head);
#endif  /*!__MACRO_PART__H__*/
