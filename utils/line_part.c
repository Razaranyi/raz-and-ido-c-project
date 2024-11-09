#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "line_part.h"
#include "commons.h"
#include "../core/command.h"




/*get char name and list of Lines, checks if the name is in the Line name list and return int as boolean*/
int in_line_table(char * name, Line * Lines)
{
	Line * temp = Lines;
	while(temp->next != NULL) /*run all over the Lines*/
	{
            if(temp->label != NULL)
            {
    		    if (strcmp(name, temp->label) == 0)
			    {
    		    	return TRUE;
			    }
            }
    		temp = temp->next;
	}
    if(temp->label != NULL)
    {
        if (strcmp(name, temp->label) == 0)
        {
            return TRUE;
        }
    }   
	return FALSE;
    
}


/*get null and return list of Lines, create the data struct*/
Line * create_line_table()
{
	Line * head = (Line*)malloc(sizeof(Line));
	check_malloc(head);
	memset(head, 0, sizeof(Line));
	return head;
	
}

/*get the data for the line columns and create new node of type Line
return pointer to Line*/
Line* createNodeLine(char * labelname, char * data, int index)
{
    Line * newNode = (Line *)malloc(sizeof(Line));
    if(newNode == NULL)
    {
        printf("Memory allocation failed");
    }
    newNode->label = (char *)malloc(strlen(labelname) + 1);
    newNode->data = (char *)malloc(strlen(data) + 1);
    strcpy(newNode->data, data);
    strcpy(newNode->label, labelname);
    newNode->index = index;
    newNode->next = NULL;
    return newNode;
}

/*get list of lines and data the line columns, use createNodeLine and append the new Node to the list
return void*/
void appendNodeLine(Line **head, char * labelname, char * data, int index)
{
    Line * newNode = createNodeLine(labelname, data, index);
    if(*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Line *current = *head;
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

/*get pointer to lines run from the head to the end and print any line
return void */
void printLines(Line * head)
{
    Line * current = head;
    while (current != NULL)
    {
        printf("Index: %d, Label: %s, Data: %s\n", current->index, current->label, current->data);
        current = current->next;
    }
    

}

/*get head to line list, run over it and free the memory*/
void free_line_table(Line * head)
{
	Line * temp = head; /*to run over the Lines*/
	while(head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
		
	}
}

