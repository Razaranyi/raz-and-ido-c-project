#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_part.h"
#include "line_part.h"
#include "commons.h"
#include "../core/command.h"



/*main function that read the file and write a new one without macros
also indexing the lines and take out the labels
get file name return bollean as int*/
int write_without_macro(char *fname, Macro ** Macros, Line ** Lines)
{
	char line [LEN_LINE]; /*for moving in the lines*/
	char clean_line [LEN_LINE]; /*to clean the line in the line indexer*/
	char macro_temp_line [LEN_LINE]; /*for macro strtok*/
	char line_temp_line [LEN_LINE]; /*for line strtok*/
	char * labelname; /*for the label name*/
	char * token; 
	int checker = TRUE; /*checks if we get problem while reading and parse the file if 0 its all good if 1 its bad*/
	int macro_close = TRUE; /*check if macro is open*/
	int linecounter = -1; /*for the line indexer*/
	char * macro_fname = (char *)malloc(strlen(fname) + 1);
	FILE* fp, *macrofile;
	strcpy(macro_fname, fname);
	add_as(fname); /*change the file name to .as*/
	add_am(macro_fname); /*change the file name to .am*/
	macrofile = fopen(macro_fname, "w");
	fp = fopen(fname, "r");	
	if (fp== NULL)/*checks if there a file*/
	{
		printf("ERROR - theres no file like %s \n", fname);
		return FALSE;
	}
	checker = getmacros(fp, Macros); /*get all the macros*/
	fclose(fp);
	if (checker == FALSE) /*checks if the macros reading went fine*/
		return FALSE;
	/*open the files*/
	fp = fopen(fname, "r");

	while (fgets(line, sizeof line, fp)!= NULL) /*run over and change the file with the known macros*/
	{
        
		strcpy(macro_temp_line, line);
		strcpy(line_temp_line, line);
		token = strtok(macro_temp_line, " \t\n");
        /*for empty line*/
        if(token == NULL)
            continue;
		if(macro_close == TRUE)
		{
            /*checks if this is start of macro*/
			if(strcmp(in_macro_table(token, *Macros), "0") != 0)
			{
				fputs(in_macro_table(token, *Macros), macrofile);
			}
            /*checks if start of macro define*/
			else if (strncmp(token, "mcro", 4) == 0 && strncmp(token, "mcroe", 5) != 0) /*start of macro*/
			{
				macro_close = FALSE;
			}
			else 
			{
				fputs(line, macrofile);
			}
		}
		else
		{
			if (strncmp(token, "mcroend", 7) == 0 && macro_close == FALSE) /*end of macro*/
			{
				macro_close = TRUE;
			}
		}	
		/*indexing the lines*/
		if(is_label(line)) /*case where the line start with label*/
		{

			labelname = strtok(line_temp_line, ":");
			cut_two_dots_start(line);
			cut_spaces(labelname);
			if (strcmp(in_macro_table(labelname, *Macros), "0") != FALSE || in_line_table(labelname, *Lines) == TRUE || is_command_name(labelname) == TRUE || check_if_instruction(labelname) == TRUE || check_if_registar(labelname) == TRUE)
			{
				checker = FALSE;
				error("ERROR - theres a problems with the label name", linecounter);
			}
		}
		else
		{
			labelname = "";
		}
		
		remove_leading_and_trailing_whitespaces(line, clean_line);
		appendNodeLine(Lines, labelname, clean_line, linecounter);
		linecounter++;
	}


    /*close and free all relevant files*/

	fclose(macrofile);
	fclose(fp);
	free(macro_fname);
	return checker;
	
}



/*get char name and list of Macros, checks if the name is in the macro name list and return the relevant data if it is,
else return "0"*/
char * in_macro_table(char * name, Macro * Macros)
{
	Macro * temp = Macros;
	while(temp->macroName != NULL) /*run all over the Macros*/
	{
    		if (strcmp(name, temp->macroName) == 0)
			{
    			return temp->data;
			}

    		temp = temp->next;
	}
	return "0";
    
}


/*get null and return list of Macros, create the data struct*/
Macro * create_macro_table()
{
	Macro * head = (Macro*)malloc(sizeof(Macro));
	check_malloc(head);
	memset(head, 0, sizeof(Macro));
	return head;
	
}

/*get a pointer to Macro list and add a new macro to it return the head of the list*/
Macro * add_macro(Macro * head)
{
	Macro * temp = head; /*to run over the Macros*/
	Macro * new_macro;
	while(temp->next != NULL)
	{
		temp = temp->next;
		
	}/*here we are in the tail of the Macros*/
	new_macro = (Macro*)malloc(sizeof(Macro));
	check_malloc(new_macro);
	memset(new_macro, 0, sizeof(Macro));
	temp->next = new_macro;
	return temp->next;
}

/*get head to macro list, run over it and free the memory*/
void free_macro_table(Macro * head)
{
	Macro * temp = head; /*to run over the Macros*/
	while(head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
		
	}/*here we are in the tail of the Macros*/
}


/*get file data and pointer to list of Macros, run over the file and get all the macros from it into the Macro list,
return boolean as int*/
int getmacros(FILE * fp, Macro ** Macros)
{
	char line [LEN_LINE]; /*for moving in the lines*/
	int checker = TRUE; /*checks if we get problem while reading and parse the file*/
	int linecounter = 0; /*counter for the lines*/
	char * token; 
	int macrocounter = 0; /*counter for the macros*/
	int macro_open = FALSE; /*checks if macro is open, if yes 1, no 0*/
	char *data; /*for the macro data*/
	char * macroname; /*for the macro name*/
	Macro * tail = *Macros;
	while (fgets(line, sizeof line, fp)!= NULL)
	{
		linecounter++;
		token = line;
		cut_spaces_start(token);
        /*for empty line*/
        if(token == NULL)
            continue;
		if (macro_open == TRUE) /*reading the macro data*/
		{
			if(strncmp(token, "mcroend", 7) != 0) /*running while mcroend not coming*/
			{
				if(data == NULL)
				{
					data = malloc(strlen(token));
					strcpy(data, token);
				}
				
                else
                { 
                    data = realloc(data, strlen(data) + strlen(token));
					strcat(data, token);	
                }
			}
			else
			{	
				cut_spaces(token);
				/*checks that there no chars after macroend*/
				if(strcmp(token, "mcroend") != 0)
				{
					checker = FALSE;
					error("ERROR - theres a char after macroend", linecounter);
				}
				(tail->data = data);
				tail = add_macro(*Macros);
				macro_open = FALSE;
				macrocounter++;
				data = NULL; /*for the macro data*/
				macroname = NULL; /*for the macro name*/
				
			}
		}
		else
		{
            /*checks if start of macro define and not the end*/
			if (strncmp(token, "mcro", 4) == 0 && strncmp(token, "mcroe", 5) != 0)
			{
				/*heading the pointer*/
				token++;
                token++;
                token++;
				token++;                
				macroname = strtok(token, " \t\0");
				cut_spaces(macroname);	
				/*checks that there no chars after macro define*/
				if (strtok(NULL, " \t\0") != NULL)
				{
					checker = FALSE;
					error("ERROR - theres a char after macro define", linecounter);

				}
				/*check valid macro name*/
				if (strcmp(in_macro_table(macroname, *Macros), "0") != FALSE || is_command_name(macroname) == TRUE || check_if_instruction(macroname) == TRUE || check_if_registar(macroname) == TRUE)
				{
					checker = FALSE;
					error("ERROR - theres a problems with the macro name", linecounter);
				}
				else /*adds the macro name*/
				{
					macro_open = TRUE;
					tail->macroName = (char *)malloc(strlen(macroname) + 1);
					strcpy(tail->macroName, macroname);
				}
			}	
		}
	}
	return checker;
}
