#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_part.h"
#include "line_part.h"
#include "commons.h"
#include "../core/command.h"
#include "../core/instruction.h"



/*main function that read the file and write a new one without macros
also indexing the lines and take out the labels
get file name return bollean as int*/
int write_without_macro(char *fname, DoublyLinkedList *macro_list, DoublyLinkedList *line_list) {
    char line[LEN_LINE];
    char clean_line[LEN_LINE];
    char macro_temp_line[LEN_LINE];
    char line_temp_line[LEN_LINE];
    char *labelname;
    char *token;
    int checker = TRUE;
    int macro_close = TRUE;
    int linecounter = -1;
    char *fname_as = add_as(fname);
    char *macro_fname_am = add_am(fname);
    FILE *fp, *macrofile;

    if (fname_as == NULL || macro_fname_am == NULL) {
        fprintf(stderr, "Memory allocation failed for filenames\n");
        free(fname_as);
        free(macro_fname_am);
        return FALSE;
    }

    macrofile = fopen(macro_fname_am, "w");
    if (macrofile == NULL) {
        fprintf(stderr, "Failed to open file %s for writing\n", macro_fname_am);
        free(fname_as);
        free(macro_fname_am);
        return FALSE;
    }

    fp = fopen(fname_as, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR - there's no file named %s\n", fname_as);
        fclose(macrofile);
        free(fname_as);
        free(macro_fname_am);
        return FALSE;
    }

    checker = get_macros(fp, macro_list);
    fclose(fp);
    if (checker == FALSE) {
        fclose(macrofile);
        free(fname_as);
        free(macro_fname_am);
        return FALSE;
    }

    /* Re-open the file after processing macros */
    fp = fopen(fname_as, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR - there's no file named %s\n", fname_as);
        fclose(macrofile);
        free(fname_as);
        free(macro_fname_am);
        return FALSE;
    }


    while (fgets(line, sizeof line, fp) != NULL) {
        strcpy(macro_temp_line, line);
        strcpy(line_temp_line, line);
        token = strtok(macro_temp_line, " \t\n");
        if (token != NULL) {
            if (macro_close == TRUE) {
                char *macro_data = in_macro_table(token, macro_list);
                if (macro_data != NULL) {
                    fputs(macro_data, macrofile);
                } else if (strncmp(token, "mcro", 4) == 0 && strncmp(token, "mcroe", 5) != 0) {
                    macro_close = FALSE;
                } else {
                    fputs(line, macrofile);
                }
            } else {
                if (strncmp(token, "mcroend", 7) == 0 && macro_close == FALSE) {
                    macro_close = TRUE;
                }
            }
            if (is_label(line)) {
                labelname = strtok(line_temp_line, ":");
                cut_two_dots_start(line);
                cut_spaces(labelname);
                if (in_macro_table(labelname, macro_list) != NULL || in_line_table(labelname, line_list) == TRUE || is_command_name(labelname) == TRUE || check_if_instruction(labelname) == TRUE || check_if_registar(labelname) == TRUE) {
                    checker = FALSE;
                    error("ERROR - there's a problem with the label name", linecounter);
                }
            } else {
                labelname = "";
            }
            remove_leading_and_trailing_whitespaces(line, clean_line);
            append_line(line_list, labelname, clean_line, linecounter);
            linecounter++;
        }
    }
    fclose(macrofile);
    fclose(fp);
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



/*get null and return list of Macros, create the data struct*/
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
    int checker = TRUE;
    int linecounter = 0;
    char *token;
    int macro_open = FALSE;
    char *data = NULL;
    char *macroname = NULL;

    while (fgets(line, sizeof line, fp) != NULL) {
        linecounter++;
        token = line;
        cut_spaces_start(token);
        if (token != NULL) {
            if (macro_open == TRUE) {
                if (strncmp(token, "mcroend", 7) != 0) {
                    /* Collect macro data*/
                    if (data == NULL) {
                        data = allocate_string(token);
                    } else {
                        char *new_data = realloc(data, strlen(data) + strlen(token) + 1);
                        if (new_data == NULL) {
                            fprintf(stderr, "Memory allocation failed during macro data concatenation\n");
                            free(data);
                            return FALSE;
                        }
                        data = new_data;
                        strcat(data, token);
                    }
                } else {
                    cut_spaces(token);
                    if (strcmp(token, "mcroend") != 0) {
                        checker = FALSE;
                        error("there's a character after 'mcroend'", linecounter);
                    }
                    /* Add macro to the list*/
                    add_macro(macro_list, macroname, data);
                    /* Reset variables for next macro*/
                    macro_open = FALSE;
                    free(data);
                    data = NULL;
                    free(macroname);
                    macroname = NULL;
                }
            } else {
                if (strncmp(token, "mcro", 4) == 0 && strncmp(token, "mcroe", 5) != 0) {
                    /* Start of a new macro */
                    macroname = allocate_string(strtok(token, " \t\0"));
                    cut_spaces(macroname);
                    if (strtok(NULL, " \t\0") != NULL) {
                        checker = FALSE;
                        error("there's a character after macro definition", linecounter);
                    }
                    if (in_macro_table(macroname, macro_list) != NULL || is_command_name(macroname) == TRUE || check_if_instruction(macroname) == TRUE || check_if_registar(macroname) == TRUE) {
                        checker = FALSE;
                        error("ERROR - there's a problem with the macro name", linecounter);
                    } else {
                        macro_open = TRUE;
                    }
                }
            }
        }
    }
    return checker;
}
