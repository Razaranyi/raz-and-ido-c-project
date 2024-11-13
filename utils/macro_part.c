#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_part.h"
#include "line_part.h"
#include "commons.h"
#include "../core/command.h"
#include "../core/instruction.h"



#include <ctype.h>  /* Include this header for isspace */


int write_without_macro(char *fname, DoublyLinkedList *macro_list, DoublyLinkedList *line_list) {
    char line[LEN_LINE];
    char clean_line[LEN_LINE];
    int checker = TRUE;
    int macro_open = FALSE;
    int linecounter = -2;
    char *fname_as = add_as(fname);
    char *macro_fname_am = add_am(fname);
    FILE *fp, *macrofile;

    if (fname_as == NULL || macro_fname_am == NULL) {
        fprintf(stderr, "Memory allocation failed for filenames\n");
        free(fname_as);
        free(macro_fname_am);
        return FALSE;
    }

    /* Open files */
    fp = fopen(fname_as, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR - there's no file named %s\n", fname_as);
        free(fname_as);
        free(macro_fname_am);
        return FALSE;
    }

    /* Collect macros */
    checker = get_macros(fp, macro_list);
    fclose(fp);
    if (checker == FALSE) {
        free(fname_as);
        free(macro_fname_am);
        fprintf(stderr, "Error in the Macros part - the program stopped\n");
        return FALSE;
    }

    /* Re-open the input file to process lines */
    fp = fopen(fname_as, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR - there's no file named %s\n", fname_as);
        free(fname_as);
        free(macro_fname_am);
        return FALSE;
    }

    /* Open the output file for writing */
    macrofile = fopen(macro_fname_am, "w");
    if (macrofile == NULL) {
        fprintf(stderr, "Failed to open file %s for writing\n", macro_fname_am);
        fclose(fp);
        free(fname_as);
        free(macro_fname_am);
        return FALSE;
    }

    /* Process each line */
    while (fgets(line, sizeof(line), fp) != NULL) {
        linecounter++;

        remove_leading_and_trailing_whitespaces(line, clean_line);

        /* Check if the line is empty */
        if (clean_line[0] == '\0') {
            fprintf(macrofile, "\n");  /* Preserve empty lines */
        } else {
            /* Declare variables at the beginning */
            char line_copy[LEN_LINE];
            char *labelname;
            char *token;
            char *line_ptr;
            char *macro_data = NULL;
            int macro_found = FALSE;
            char *macro_name = NULL;
            char *rest_of_line = NULL;

            strcpy(line_copy, clean_line);
            token = strtok(line_copy, " \t\n");

            if (macro_open == TRUE) {
                /* Check for "mcroend" */
                if (token != NULL && strcmp(token, "mcroend") == 0) {
                    macro_open = FALSE;
                }
                /* Do not write macro definitions to the output */
            } else {
                /* Check for macro definition start */
                if (token != NULL && strcmp(token, "mcro") == 0) {
                    macro_open = TRUE;
                    /* Do not write macro definitions to the output */
                } else {
                    /* Not inside macro definition, process tokens */
                    line_ptr = line_copy;  /* Reset pointer for strtok */
                    token = strtok(line_ptr, " \t\n");
                    while (token != NULL) {
                        macro_data = in_macro_table(token, macro_list);
                        if (macro_data != NULL) {
                            macro_found = TRUE;
                            macro_name = token;
                            /* Get rest of line after macro name */
                            rest_of_line = clean_line + (token - line_copy) + strlen(token);
                            /* Remove leading whitespace from rest_of_line */
                            while (isspace((unsigned char)*rest_of_line)) rest_of_line++;
                            break;
                        }
                        token = strtok(NULL, " \t\n");
                    }
                    if (macro_found) {
                        /* Declare variables at the beginning */
                        int macro_name_pos;
                        char before_macro[LEN_LINE];
                        char trimmed_before_macro[LEN_LINE];
                        char *macro_data_copy;
                        char *line_start;
                        char *newline_pos;
                        char *last_line = NULL;

                        /* Expand macro */
                        /* Write any text before macro name */
                        macro_name_pos = macro_name - line_copy;
                        strncpy(before_macro, clean_line, macro_name_pos);
                        before_macro[macro_name_pos] = '\0';

                        /* Remove trailing whitespace from before_macro */
                        remove_leading_and_trailing_whitespaces(before_macro, trimmed_before_macro);

                        if (trimmed_before_macro[0] != '\0') {
                            fprintf(macrofile, "%s ", trimmed_before_macro);
                        }

                        /* Write macro data line by line */
                        macro_data_copy = allocate_string(macro_data);
                        if (macro_data_copy == NULL) {
                            fprintf(stderr, "Memory allocation failed\n");
                            fclose(fp);
                            fclose(macrofile);
                            free(fname_as);
                            free(macro_fname_am);
                            return FALSE;
                        }

                        line_start = macro_data_copy;
                        while ((newline_pos = strchr(line_start, '\n')) != NULL) {
                            *newline_pos = '\0'; /* Replace newline with null terminator */
                            fprintf(macrofile, "%s\n", line_start);
                            line_start = newline_pos + 1;
                        }
                        /* Handle last line */
                        if (*line_start != '\0') {
                            last_line = line_start;
                        }

                        if (last_line != NULL) {
                            if (rest_of_line != NULL && *rest_of_line != '\0') {
                                fprintf(macrofile, "%s %s\n", last_line, rest_of_line);
                            } else {
                                fprintf(macrofile, "%s\n", last_line);
                            }
                        }
                        free(macro_data_copy);
                    } else {
                        /* No macro invocation found; write line as is */
                        fprintf(macrofile, "%s\n", clean_line);
                    }
                }
            }
			
			if(is_label(clean_line)) /*case where the line start with label*/
			{
				strcpy(line_copy, clean_line);
				labelname = strtok(line_copy, ":");
				cut_two_dots_start(clean_line);
				cut_spaces(labelname);
				if (in_macro_table(labelname, macro_list) || in_line_table(labelname, line_list) == TRUE || is_command_name(labelname) == TRUE || check_if_instruction(labelname) == TRUE || check_if_registar(labelname) == TRUE)
				{
					checker = FALSE;
					error("ERROR - theres a problems with the label name", linecounter);
					labelname = "";
				}

			}
			else
			{
				labelname = "";
			}
			/*adding line to the line list*/
			remove_leading_and_trailing_whitespaces(clean_line, clean_line);
			append_line(line_list, labelname, clean_line, linecounter);

		}
    }
    
    printf("finished writing %s...\n",macro_fname_am);

    /* Close files and free memory */
    fclose(fp);
    fclose(macrofile);
    free(fname_as);
    free(macro_fname_am);

    return checker;
}







/*get char name and list of Macros, checks if the name is in the macro name list and return the relevant data if it is,
else return "0"*/
char* in_macro_table(char *name, DoublyLinkedList *macro_list) {
    DoublyLinkedList *current = get_list_head(macro_list);
    while (current != NULL) {
        if (current->data != NULL) {
            Macro *macro = (Macro *)current->data;
            if (macro->macroName != NULL && strcmp(name, macro->macroName) == 0) {
                return macro->data;
            }
        }
        current = current->next;
    }
    return NULL;
}



/*return list of Macros, create the data struct*/
DoublyLinkedList* create_macro_table(){
    return allocate_node_mem();
}

/*get a pointer to Macro list and add a new macro to it return the head of the list*/
void add_macro(DoublyLinkedList *macro_list, char *macroName, char *data) {
    Macro *new_macro = (Macro *)malloc(sizeof(Macro));
    if (new_macro == NULL) {
        fprintf(stderr, "Memory allocation failed for Macro\n");
        return;
    }
    new_macro->macroName = allocate_string(macroName);
    new_macro->data = allocate_string(data);
    add_to_list(macro_list, new_macro);
}


/*get head to macro list, run over it and free the memory*/
void free_macro_data(void *data) {
    Macro *macro = (Macro *)data;
    if (macro != NULL) {
        free(macro->macroName);
        free(macro->data);
        free(macro);
    }
}

void free_macro_table(DoublyLinkedList *macro_list) {
    free_list(&macro_list, free_macro_data);
}


/*get file data and pointer to list of Macros, run over the file and get all the macros from it into the Macro list,
return boolean as int*/
int get_macros(FILE *fp, DoublyLinkedList *macro_list) {
    char line[LEN_LINE];
    char clean_line[LEN_LINE];  /* For storing the line after removing whitespace */
    int checker = TRUE;
    int linecounter = 0;
    int macro_open = FALSE;
    char *data = NULL;
    char *macroname = NULL;

    while (fgets(line, sizeof(line), fp) != NULL) {
        linecounter++;

        /* Remove leading and trailing whitespace using your function */
        remove_leading_and_trailing_whitespaces(line, clean_line);

        /* Process the line only if it's not empty or a comment */
        if (!(clean_line[0] == '\0' || clean_line[0] == ';')) {

            if (macro_open == TRUE) {
                /* Inside a macro definition */

                /* Check for "mcroend" */
                char *token = strtok(clean_line, " \t\n");
                if (token != NULL && strcmp(token, "mcroend") == 0) {
                    /* Check for extra tokens after "mcroend" */
                    if (strtok(NULL, " \t\n") != NULL) {
                        checker = FALSE;
                        error("there's a character after 'mcroend'", linecounter);
                    } else {
                        /* Add macro to the list */
                        add_macro(macro_list, macroname, data);

                        /* Reset variables for next macro */
                        macro_open = FALSE;
                        free(data);
                        data = NULL;
                        free(macroname);
                        macroname = NULL;
                    }
                } else {
                    /* Collect macro data */
                    size_t line_len = strlen(clean_line);
                    if (data == NULL) {
                        data = allocate_string(clean_line);
                    } else {
                        /* Reallocate data to accommodate new line plus newline character */
                        size_t new_size = strlen(data) + line_len + 2; /* +2 for newline and null terminator */
                        char *new_data = realloc(data, new_size);
                        if (new_data == NULL) {
                            fprintf(stderr, "Memory allocation failed during macro data concatenation\n");
                            free(data);
                            return FALSE;
                        }
                        data = new_data;
                        strcat(data, "\n");
                        strcat(data, clean_line);
                    }
                }
            } else {
                /* Not inside a macro definition */

                /* Check for macro definition start */
                char *token = strtok(clean_line, " \t\n");
                if (token != NULL && strcmp(token, "mcro") == 0) {
                    /* Get macro name */
                    char *name_token = strtok(NULL, " \t\n");
                    if (name_token == NULL) {
                        checker = FALSE;
                        error("Macro name is missing", linecounter);
                    } else {
                        /* Check for extra tokens after macro name */
                        if (strtok(NULL, " \t\n") != NULL) {
                            checker = FALSE;
                            error("there's a character after macro definition", linecounter);
                        } else {
                            /* Validate macro name */
                            if (in_macro_table(name_token, macro_list)){
                                checker = FALSE;
                                errorf(linecounter,"macro %s is already defined",name_token);
                            }
                            else if (is_command_name(name_token) ||
                                     check_if_instruction(name_token) ||
                                     check_if_registar(name_token))
                                 {
                                checker = FALSE;
                                errorf(linecounter,
                                       "macro can't be the same as command, instruction or register. Please fix macro name: %s"
                                       , linecounter);
                            } else {
                                macro_open = TRUE;
                                /* Allocate and copy macro name */
                                macroname = allocate_string(name_token);
                                data = NULL;
                            }
                        }
                    }
                }
            }

        } else {
            /* Line is empty or a comment; do nothing */
            /*TODO: make sure that those lines are calculated on error prompt*/
        }
    }

    /* Check if a macro was left open */
    if (macro_open == TRUE) {
        checker = FALSE;
        error("Macro not closed before end of file", linecounter);
        /* Free allocated memory */
        free(data);
        free(macroname);
    }

    return checker;
}
