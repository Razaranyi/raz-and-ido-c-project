#include "create_files.h"
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
#include "../exec/compiler.h"

/*todo: changes the address part for list options*/

/*get double linked list of encode line pairs, run over it and for any line enter her hex representation to the object file*/
void create_object_file(DoublyLinkedList *encode_line_pair)
{
    FILE *f; /*to write in*/
    const char *filename = "ps.ob";
    DoublyLinkedList *current = get_list_head(encode_line_pair);
    AddressEncodedPair *pair;

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
    fclose(f);
}



/*get double linked list symbol table, run over it and for entry symbols write to ps.ent file*/
void create_entry_file(DoublyLinkedList *symbol_table)
{
    FILE *f; /*to write in*/
    const char *filename = "ps.ent";
    int first_entry = TRUE; /*to open the file*/
	DoublyLinkedList* new_symbol = get_list_head(symbol_table);
    char * label; /*for label part*/
    char * address; /*for the addresss writing part*/
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
}



/*get double linked list symbol table, run over it and for extern symbols write to ps.ext file*/
void create_extern_file(DoublyLinkedList *symbol_table)
{
    FILE *f; /*to write in*/
    const char *filename = "ps.ext";
    int first_extern = TRUE; /*to open the file*/
	DoublyLinkedList* new_symbol = get_list_head(symbol_table);
    DoublyLinkedList* address; /*for the external multipule address*/
    char * label; /*for label part*/
    unsigned long *current_address; /*for the addresss writing part*/
    char * fixed_address; /*for the fixed address*/
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
}


/*grouping all the create files (extern, entry, object) function, run it and create 3 files*/
void create_files(DoublyLinkedList *symbol_table,DoublyLinkedList *encode_line_pair)
{
    create_entry_file(symbol_table);
    create_extern_file(symbol_table);
    create_object_file(encode_line_pair);
}