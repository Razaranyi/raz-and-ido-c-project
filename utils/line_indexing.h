#ifndef __LINE_PART__H__
#define __LINE_PART__H__

#include "../core/doubly_linked_list.h"
#include "commons.h"
#include "../core/command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**Line structure including label, the content (data) and the line number (index) of the line*/
typedef struct {
    char *label;
    char *data;
    int index;
} Line;



/**create new line table for the Line section*/
DoublyLinkedList* create_line_table();

/**get the data for the line columns
return pointer to new Line*/
Line* createNodeLine( char *labelname,  char *data, int index);

/**get list of lines and data the line columns
return void*/
void append_line(DoublyLinkedList *lineList,  char *labelname, char *data, int index);
/**get pointer to lines
return void */
void print_lines(DoublyLinkedList *line_list);

/**check if name is in the line table if yes return int as boolean*/
int in_line_table(char *name, DoublyLinkedList *line_list);

/**run over the lines table and free it*/
void free_line_table(DoublyLinkedList* line_list);

#endif
