#include "line_indexing.h"

/*get char name and list of Lines, checks if the name is in the Line name list and return int as boolean*/
int in_line_table(char *name, DoublyLinkedList *line_list) {
    DoublyLinkedList *current = get_list_head(line_list);
    while (current != NULL && current->data != NULL) {
        Line *line = (Line *) current->data;
        if (line->label != NULL && strcmp(name, line->label) == 0) {
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}



/*get the data for the line columns and create new node of type Line
return pointer to Line*/
Line *createNodeLine(char *labelname, char *data, int index) {
    Line *newNode = (Line *) malloc(sizeof(Line));
    if (newNode == NULL) {
        errorf(index,"Memory allocation failed for Line");
        return NULL;
    }

    newNode->label = (char *) malloc(strlen(labelname) + 1);
    if (newNode->label == NULL) {
        errorf(index,"Memory allocation failed for Line");
        free(newNode);
        return NULL;
    }
    strcpy(newNode->label, labelname);

    newNode->data = (char *) malloc(strlen(data) + 1);
    if (newNode->data == NULL) {
        errorf(index,"Memory allocation failed for Line");
        free(newNode->label);
        free(newNode);
        return NULL;
    }
    strcpy(newNode->data, data);

    newNode->index = index;

    return newNode;
}


/*get list of lines and data the line columns, use createNodeLine and append the new Node to the list
return void*/
void append_line(DoublyLinkedList *lineList, char *labelname, char *data, int index) {
    Line *newLine = createNodeLine(labelname, data, index);
    if (newLine == NULL) {
        errorf(index,"Can't create node");
        return;
    }
    add_to_list(lineList, newLine);
}


/*get pointer to doublyLinkedList (lines) run from the head to the end and print any line
return void */
void print_lines(DoublyLinkedList *line_list) {
    DoublyLinkedList *current = get_list_head(line_list);
    while (current != NULL) {
        Line *line = (Line *) current->data;
        printf("Index: %d, Label: %s, Data: %s\n", line->index, line->label, line->data);
        current = current->next;
    }

}


/*get head to line list, run over it and free the memory*/
void free_line_data(void *data) {
    Line *line = (Line *) data;
    if (line) {
        free(line->label);
        free(line->data);
        free(line);
    }
}

void free_line_table(DoublyLinkedList *line_list) {
    free_list(&line_list, free_line_data);
}


