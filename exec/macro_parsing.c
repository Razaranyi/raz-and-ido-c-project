#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_parsing.h"
#include "../utils/line_indexing.h"
#include "../utils/commons.h"
#include "../core/command.h"
#include "../core/instruction.h"


#include <ctype.h>  /* Include this header for isspace */


int parse_macro(char *fname, DoublyLinkedList *macro_list, DoublyLinkedList *line_list) {
    char line[LEN_LINE];
    char clean_line[LEN_LINE];
    int checker = TRUE;
    int macro_open = FALSE;
    int macro_index = 0;
    int linecounter = 0;
    int index_to_write = 0;
    char *fname_as = add_end_to_filename(fname, ".as");
    char *macro_fname_am = add_end_to_filename(fname, ".am");
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

        /* Check if the line is empty and avoid deleting it */
        if (clean_line[0] == '\0') {
            fprintf(macrofile, "\n");
        } else {
            char line_copy[LEN_LINE];
            char *labelname;
            char *token;
            char *macro_data = NULL;
            int macro_found = FALSE;
            char *macro_name = NULL;


            strcpy(line_copy, clean_line);
            token = strtok(line_copy, " \t\n");

            if (macro_open == TRUE) {
                /* Check for "mcroend" */
                if (token != NULL && strcmp(token, "mcroend") == 0) {
                    macro_open = FALSE;
                }
            } else {
                /* Check for macro definition start */
                if (token != NULL && strcmp(token, "mcro") == 0) {
                    macro_open = TRUE;
                } else {
                    /* Not inside macro definition, process tokens */

                    macro_data = in_macro_table(token, macro_list);
                    macro_name = token;
                    token = strtok(NULL, " \t\n");

                    if (macro_data != NULL  && token == NULL)
                    {
                        macro_found = TRUE;  
                        macro_index = get_index_macro_table(macro_name, macro_list);

                    }
                        
                    
                    if (macro_found) {
                        /* Declare variables at the beginning */
                        int macro_name_pos;
                        char * newline_pos;
                        char before_macro[LEN_LINE];
                        char trimmed_before_macro[LEN_LINE];
                        char *macro_data_copy;
                        char *line_start;
                        char *last_line = NULL;
                        /* Expand macro */
                        /* Write any text before macro name */
                        macro_name_pos = macro_name - line_copy;
                        strncpy(before_macro, clean_line, macro_name_pos);
                        before_macro[macro_name_pos] = '\0';

                        /* Remove trailing whitespace from before_macro */
                        remove_leading_and_trailing_whitespaces(before_macro, trimmed_before_macro);


                        if(is_string_begin_with_substring(macro_name,";")){
                            warnf(linecounter,
                                  "Macro %s is starting with ';' but it's not a comment and will be parsed and processed!",
                                  macro_name);
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
                        while ((newline_pos = strchr(line_start, '\n')) != NULL) 
                        {
                            *newline_pos = '\0'; /* Replace newline with null terminator */
                            index_to_write = ++macro_index;
                            strcpy(clean_line, line_start);
                            if (is_label(clean_line)) /*case where the line start with label*/
                            {
                                strcpy(line_copy, clean_line);
                                labelname = strtok(line_copy, ":");
                                cut_two_dots_start(clean_line);
                                cut_spaces(labelname);
                                if (in_macro_table(labelname, macro_list) || in_line_table(labelname, line_list) == TRUE ||
                                    is_command_name(labelname) || get_instruction_enum(labelname) ||
                                        is_register(labelname)) {
                                    checker = FALSE;
                                    errorf(linecounter,"ERROR - theres a problems with the label name" );
                                    labelname = "";
                                }

                            } else {
                                labelname = "";
                            }
                            /*adding line to the line list*/
                            remove_leading_and_trailing_whitespaces(clean_line, clean_line);
                            append_line(line_list, labelname, clean_line, index_to_write);
                            fprintf(macrofile, "%s\n", line_start);
                            line_start = newline_pos + 1;
                        }
                        /* Handle last line */
                        if (*line_start != '\0') {
                            last_line = line_start;
                        }
                        if (last_line != NULL) {

                            fprintf(macrofile, "%s\n", last_line);
                            
                            index_to_write = ++macro_index;
                            strcpy(clean_line, last_line);
                            if (is_label(clean_line)) /*case where the line start with label*/
                            {
                                strcpy(line_copy, clean_line);
                                labelname = strtok(line_copy, ":");
                                cut_two_dots_start(clean_line);
                                cut_spaces(labelname);
                                if (in_macro_table(labelname, macro_list) || in_line_table(labelname, line_list) ||
                                    is_command_name(labelname)|| get_instruction_enum(labelname) ||
                                    is_register(labelname)) {
                                    checker = FALSE;
                                    errorf(linecounter,"ERROR - theres a problems with the label name");
                                    labelname = "";
                                }

                            } else {
                                labelname = "";
                            }
                            remove_leading_and_trailing_whitespaces(clean_line, clean_line);
                            append_line(line_list, labelname, clean_line, index_to_write);
                        }

                        free(macro_data_copy);
                        index_to_write = ++macro_index;
                    } else {
                        index_to_write = linecounter;
                        /* No macro invocation found; write line as is */
                        fprintf(macrofile, "%s\n", clean_line);
                        if (is_label(clean_line)) /*case where the line start with label*/
                        {
                            strcpy(line_copy, clean_line);
                            labelname = strtok(line_copy, ":");
                            cut_two_dots_start(clean_line);
                            cut_spaces(labelname);

                        } else {
                            labelname = "";
                        }
                        /*adding line to the line list*/
                        remove_leading_and_trailing_whitespaces(clean_line, clean_line);
                        append_line(line_list, labelname, clean_line, index_to_write);
                    }
                    }
            }



        }
    }

    infof(-1,"Finished writing %s...", macro_fname_am);

    /* Close files and free memory */
    fclose(fp);
    fclose(macrofile);
    free(fname_as);
    free(macro_fname_am);

    return checker;
}


/*get char name and list of Macros, checks if the name is in the macro name list and return the relevant data if it is,
else return "0"*/
char *in_macro_table(char *name, DoublyLinkedList *macro_list) {
    DoublyLinkedList *current = get_list_head(macro_list);
    while (current != NULL) {
        if (current->data != NULL) {
            Macro *macro = (Macro *) current->data;
            if (macro->macroName != NULL && strcmp(name, macro->macroName) == 0) {
                return macro->data;
            }
        }
        current = current->next;
    }
    return NULL;
}



/*get char name and list of Macros, checks if the name is in the macro name list and return the relevant macro index,
else return NULL*/
int get_index_macro_table(char *name, DoublyLinkedList *macro_list) {
    DoublyLinkedList *current = get_list_head(macro_list);
    while (current != NULL) {
        if (current->data != NULL) {
            Macro *macro = (Macro *) current->data;
            if (macro->macroName != NULL && strcmp(name, macro->macroName) == 0) {
                return macro->macroIndex;
            }
        }
        current = current->next;
    }
    return -1;
}




/*get a pointer to Macro list and add a new macro to it return the head of the list*/
void add_macro(DoublyLinkedList *macro_list, char *macroName, char *data, int linecounter) {
    Macro *new_macro = (Macro *) malloc(sizeof(Macro));
    if (new_macro == NULL) {
        fprintf(stderr, "Memory allocation failed for Macro\n");
        return;
    }
    new_macro->macroName = allocate_string(macroName);
    new_macro->data = allocate_string(data);
    new_macro->macroIndex = linecounter;
    add_to_list(macro_list, new_macro);

}


/*get head to macro list, run over it and free the memory*/
void free_macro_data(void *data) {
    Macro *macro = (Macro *) data;
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
    char clean_line[LEN_LINE];
    char temp_line[LEN_LINE];
    int checker = TRUE;
    int linecounter = 0;
    int macroindex = 0;
    int macro_open = FALSE;
    char *data = NULL;
    char *token;
    char *macroname = NULL;

    while (fgets(line, sizeof(line), fp) != NULL) {
        linecounter++;
        remove_leading_and_trailing_whitespaces(line, clean_line);

        if (clean_line[0] != '\0' && clean_line[0] != ';') {
            if (macro_open) {
                strcpy(temp_line, clean_line);
                token = strtok(temp_line, " \t\n");
                if (token != NULL && strcmp(token, "mcroend") == 0) {
                    /* Check for extra tokens after "mcroend" */
                    if (strtok(NULL, " \t\n") != NULL) {
                        checker = FALSE;
                        errorf(linecounter,"There's a character after 'mcroend'");
                    } else {
                        if (data == NULL || strlen(data) == 0 || is_all_whitespace(data)) {
                            warnf(linecounter, "Empty macro - macro '%s' is defined but has no content", macroname);
                            add_macro(macro_list, macroname, data, macroindex);
                        } else {
                            add_macro(macro_list, macroname, data, macroindex);
                        }
                        /* Reset variables for next macro */
                        macro_open = FALSE;
                        free(macroname);
                        free(data);
                        data = NULL;
                        macroname = NULL;
                    }
                }
                else {
                    size_t line_len = strlen(clean_line);
                    if (data == NULL) {
                        data = allocate_string(clean_line);
                    } else {
                        size_t new_size = strlen(data) + line_len + 2;
                        char *new_data = realloc(data, new_size);
                        if (!new_data) {
                            checker = FALSE;
                            errorf(linecounter,"Memory allocation failed during macro data concatenation");
                            free(data);
                            return FALSE;
                        }
                        data = new_data;
                        strcat(data, "\n");
                        strcat(data, clean_line);
                    }
                }
            } else {
                char *token = strtok(clean_line, " \t\n");
                if (token && strcmp(token, "mcro") == 0) {
                    char *name_token = strtok(NULL, " \t\n");
                    macroindex = linecounter;
                    if (!name_token) {
                        checker = FALSE;
                        errorf(linecounter,"Macro name is missing");
                    } else if (in_macro_table(name_token, macro_list) ||
                               is_command_name(name_token) ||
                            get_instruction_enum(name_token) ||
                               is_register(name_token)) {
                        checker = FALSE;
                        errorf(linecounter, "Invalid macro name: %s", name_token);
                    } else {
                        macro_open = TRUE;
                        macroname = allocate_string(name_token);
                        data = NULL;
                    }
                }
            }
        }
    }

    if (macro_open) {
        checker = FALSE;
        errorf(linecounter,"Unclosed macro at end of file");
        if (data != NULL){
            free(data);
        }
        free(macroname);
    }

    return checker;
}

