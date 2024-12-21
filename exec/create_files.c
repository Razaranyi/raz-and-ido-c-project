#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../exec/create_files.h"
#include "../core/doubly_linked_list.h"
#include "../core/symbol.h"
#include "../core/symbol.h"
#include "../core/instruction.h"
#include "../core/command.h"
#include "../core/operand.h"
#include "../utils/logger.h"
#include "../utils/commons.h"
#include "../core/doubly_linked_list.h"
#include "../utils/boolean.h"
#include "../core/address_encoded_pair.h"
#include "compiler.h"


/*get dir and file name and return the full path*/
char *concat_dir_and_fname(char *dir, char *fname)
{
    size_t dir_length, total_length;
    char *path, *slash;
    /*if one of them NULL return NULL*/
    if (dir == NULL || fname == NULL)
    {
        return NULL; 
    }
    dir_length = strlen(dir);
    /*get the full length*/
    total_length = dir_length + strlen(fname) + ((dir_length > 0) ? 1 : 0) + 1;
    path = (char *)malloc(total_length);
    slash = "/";
    if (path == NULL)
    {
        printf("Malloc error");
        exit(1);
    }
    /*building the full path*/
    if (dir_length > 0)
    {
        strcpy(path, dir);
        strcat(path, slash);
        strcat(path, fname);
    } 
    else
    {
        strcpy(path, fname);
    }
    return path;
}

/*get file path and return the file dir*/
char *get_dir_path(char *file_path) 
{
    size_t dir_length;
    char * last_slash, *dir;
    if (file_path == NULL)
    {
        return NULL; 
    }
    last_slash = strrchr(file_path, '/');
    if (last_slash == NULL) /*checks if only file name without path*/
    {
        return ""; 
    }
    dir_length = last_slash - file_path;
    dir = (char *)malloc(dir_length + 1);
    if (dir == NULL)
    {
        printf("Malloc error");
        exit(1);
    }
    strncpy(dir, file_path, dir_length);
    dir[dir_length] = '\0'; 
    return dir;
}


/*get double linked list of encode line pairs and dir to the file, run over it and for any line enter her hex representation to the object file*/
void create_object_file(DoublyLinkedList *encode_line_pair, char * dir)
{
    FILE *f; /*to write in*/
    char *filename = "ps.ob";
    DoublyLinkedList *current = get_list_head(encode_line_pair);
    AddressEncodedPair *pair;
    filename = concat_dir_and_fname(dir, filename);
    if (current == NULL) {
        printf("The list is empty.\n");
        return;
    }
    f = fopen(filename, "w");
    if(f == NULL)
    {
        printf("Error opening file: %s", filename);
        exit(1);
    }
    fprintf(f, "% 7ld  %ld\n", final_IC, final_DC);
    while (current != NULL) {
        pair = (AddressEncodedPair *)current->data;

        if (pair != NULL && pair->encoded_line != NULL) {
            fprintf(f, "%07lu  %06x\n", pair->address, get_encoded_line_as_binary(pair->encoded_line));
        }
        current = current->next;
    }
    free(filename);
    fclose(f);
}



/*get double linked list symbol table and dir to the file, run over it and for entry symbols write to ps.ent file*/
void create_entry_file(DoublyLinkedList *symbol_table, char * dir)
{
    FILE *f; /*to write in*/
    char *filename = "ps.ent";
    int first_entry = TRUE; /*to open the file*/
	  DoublyLinkedList* new_symbol = get_list_head(symbol_table);
    char * label; /*for label part*/
    char * address; /*for the addresss writing part*/
    filename = concat_dir_and_fname(dir, filename);
    while(new_symbol!=NULL)
    {
        Symbol * current_symbol = new_symbol->data; 
	    DoublyLinkedList * properties = current_symbol->sym_properties;
	    int if_entry = FALSE;
        while(properties!=NULL)
        {
            
            int *prop = properties->data;
	        if (*prop == ENTRY_PROPERTY)
            {
                if_entry = TRUE;
            }
            properties = properties->next;
        }
        if(if_entry)
        {
            if(first_entry) /*open the file with w*/
            {
                first_entry = FALSE; /*to not open again*/
                f = fopen(filename, "w");
                if(f == NULL)
                {
                    printf("Error opening file: %s", filename);
                    exit(1);
                }
            }

            f = fopen(filename, "a");
            if(f == NULL)
            {
                printf("Error opening file: %s", filename);
                exit(1);
            }
            label = current_symbol->label;
            address = fix_address(current_symbol->address);
            fprintf(f, "%s %s\n", label, address);
            free(address);
            fclose(f);
        }
        
        new_symbol = new_symbol->next;
    }
    free(filename);
}



/*get double linked list symbol table and dir to the file, run over it and for extern symbols write to ps.ext file*/
void create_extern_file(DoublyLinkedList *symbol_table, char * dir)
{
    FILE *f; /*to write in*/
    char *filename = "ps.ext";
    int first_extern = TRUE; /*to open the file*/
	  DoublyLinkedList* new_symbol = get_list_head(symbol_table);
    DoublyLinkedList* address; /*for the external multipule address*/
    char * label; /*for label part*/
    unsigned long *current_address; /*for the addresss writing part*/
    char * fixed_address; /*for the fixed address*/
    filename = concat_dir_and_fname(dir, filename);
    while(new_symbol!=NULL)
    {
        Symbol * current_symbol = new_symbol->data; 
	    DoublyLinkedList * properties = current_symbol->sym_properties;
	    int if_extern = FALSE;
        while(properties!=NULL)
        {
            
            int *prop = properties->data;
	        if (*prop == EXTERNAL_PROPERTY)
            {
                if_extern = TRUE;
            }
            properties = properties->next;
        }
        if(if_extern)
        {
            if(first_extern) /*open the file with w*/
            {
                first_extern = FALSE; /*to not open again*/
                f = fopen(filename, "w");
                if(f == NULL)
                {
                    printf("Error opening file: %s", filename);
                    exit(1);
                }
            }

            label = current_symbol->label;
            address = get_list_head(current_symbol->external_usages);
            while(address!=NULL)
            {
                f = fopen(filename, "a");
                if(f == NULL)
                {
                    printf("Error opening file: %s", filename);
                    exit(1);
                }
                current_address = (unsigned long *)(address->data);
                fixed_address = fix_address(*current_address);
                fprintf(f, "%s %s\n", label, fixed_address);
                free(fixed_address);
                fclose(f);
                address = address->next;
            }
        }
        new_symbol = new_symbol->next;
    }
    free(filename);
}


/*grouping all the create files (extern, entry, object) function, run it and create 3 files*/
void create_files(DoublyLinkedList *symbol_table,DoublyLinkedList *encode_line_pair, char * fname)
{
    char * dir;
    infof(-1,"Starting to write files...");
    dir = get_dir_path(fname);
    create_entry_file(symbol_table, dir);
    create_extern_file(symbol_table, dir);
    create_object_file(encode_line_pair, dir);
    free(dir);
}