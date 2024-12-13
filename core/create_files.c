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





/*get double linked list symbol table, run over it and for entry symbols write to ps.ent file*/
void create_entry_file(DoublyLinkedList *symbol_table)
{
    FILE *f; /*to write in*/
    const char *filename = "ps.ent";
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

            f = fopen(filename, "a");
            if(f == NULL)
            {
                printf("Error opening file: %s", filename);
                exit(1);
            }
            label = current_symbol->label;
            address = fix_address(current_symbol->address);
            fprintf(f, "%s %s\n", label, address);
            fclose(f);
        }
        
        new_symbol = new_symbol->next;
    }
}



/*get double linked list symbol table, run over it and for entry symbols write to ps.ent file*/
void create_extern_file(DoublyLinkedList *symbol_table)
{
    FILE *f; /*to write in*/
    const char *filename = "ps.ext";
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
	        if (*prop == EXTERNAL_PROPERTY)
            {
                if_entry = TRUE;
            }
            properties = properties->next;
        }
        if(if_entry)
        {

            f = fopen(filename, "a");
            if(f == NULL)
            {
                printf("Error opening file: %s", filename);
                exit(1);
            }
            label = current_symbol->label;
            address = fix_address(current_symbol->address);
            fprintf(f, "%s %s\n", label, address);
            fclose(f);
        }
        
        new_symbol = new_symbol->next;
    }
}

