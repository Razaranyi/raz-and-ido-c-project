#ifndef __LINE_PART__H__
#define __LINE_PART__H__



/*struct for line table*/
typedef struct Line_t{
	char * label;
	char * data;
	int index;
    struct Line_t * next;
} Line;


/*create new line table for the Line section*/
Line * create_line_table();

/*get the data for the line columns
return pointer to new Line*/
Line* createNodeLine(char * labelname, char * data, int index);

/*get list of lines and data the line columns
return void*/
void appendNodeLine(Line **head, char * labelname, char * data, int index);

/*get pointer to lines
return void */
void printLines(Line * head);


/*check if name is in the line table if yes return int as boolean*/
int in_line_table(char * name, Line * Lines);

/*run over the lines table and free it*/
void free_line_table(Line * head);
#endif  /*!__Line_PART__H__*/
