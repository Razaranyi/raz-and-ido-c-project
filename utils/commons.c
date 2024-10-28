#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <ctype.h>
#include "commons.h"
#include "../core/command.h"
#include "../core/doubly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*get pointer and checks if malloc worked OK*/
void check_malloc(void * p)
{
	if(p == NULL)
	{
		printf("Memory allocation failed. \n");
		exit(0); 
	}
}


/*get word, list of words and list of words size , run over the list and check if the word is in the list
return bollean int for true or false*/
int is_word_in_list(char *word, char *word_list[], int list_size)
{
    int i;
    cut_spaces(word);
    for (i = 0; i < list_size; i++)
    {
        if (strcmp(word, word_list[i]) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}



/*get word
with macro INSTRUCTIONS and is_word_in_list checks if the word is in the instruction list
return int as boolean*/
int check_if_instruction(char *word)
{
    char *instructions[INSTRUCTIONS_LEN] = INSTRUCTIONS;
    return is_word_in_list(word, instructions, INSTRUCTIONS_LEN);
}


/*get word
with macro REGS and is_word_in_list checks if the word is in the regs list
return int as boolean*/
int check_if_registar(char *word)
{
    char *regs[REGS_LEN] = REGS;
    return is_word_in_list(word, regs, REGS_LEN);
}



/*get char * (fname)
add to it ".as" in tne end
return char* */
char * add_as(char* fname)
{

	size_t len = strlen(fname); /*get the length of the original string*/
	fname = realloc(fname, len + 4); /*allocate memory for the one with the ".as"*/
	if (fname == NULL)
	{
		printf("Memory allocation failed. \n");
		return NULL;
	}
	strcat(fname, ".as");
	return fname;
}


/*get char * (fname)
add to it ".am" in tne end
return char* */
char * add_am(char* fname)
{
	size_t len = strlen(fname); /*get the length of the original string*/
	fname = realloc(fname, len + 4*sizeof(char)); /*allocate memory for the one with the ".as"*/
	if (fname == NULL)
	{
		printf("Memory allocation failed. \n");
		return NULL;
	}
	strcat(fname, ".am");
	return fname;
}

/*get input and remove the whitespaces in word
return void, change the input*/
void cut_spaces(char * input)
{
    int i = 0, j =0;
    while(input[i] != '\0')
    {
        if(!isspace(input[i])) /*pass on space indexs*/
        {
            input[j++] = input[i];
        }
        i++;
    }
    input[j] = '\0';
    
}

/*cut out vhars from the begging of string till "":"
return void, change the input*/
void cut_two_dots_start(char * input)
{
    int i = 0, j =0;
    /*till space*/
    while(input[i] != ':')
    {
        i++;
    }
    i++;
    /*till end*/
    while(input[i] != '\0')
    {
        input[j++] = input[i++]; 
    }
    input[j] = '\0';
    
}
/*get input and remove the whitespaces from the start of the word
return void, change the input*/
void cut_spaces_start(char * input)
{
    int i = 0, j =0;
    /*till space*/
    while(isspace(input[i]))
    {
        i++;
    }
    /*till end*/
    while(input[i] != '\0')
    {
        input[j++] = input[i++]; 
    }
    input[j] = '\0';
    
}






char* allocate_string(char* string){
    int str_len = strlen(string);
    char* res = malloc(str_len + 1);
    strcpy(res,string);
    return  res;
}

int* allocate_int(int integer){
    int* ptr = malloc(sizeof (int));
    *ptr = integer;
    return ptr;
}

int is_string_begin_with_substring(char* string, char* substring){
    return strstr(string,substring) == string;
}

int is_label( char* line) {
    /* Pattern to match a label: a valid label name ending with ':' */
    char* pattern = "^[a-zA-Z][a-zA-Z0-9].*:.*";
    return is_string_equal_by_regex(line, pattern);
}


int is_string_equal_by_regex(char* string, char* pattern) {
    regex_t reg;
    int res;
    /* compile the regular expression */
    if (regcomp(&reg, pattern, REG_EXTENDED | REG_ICASE) != 0) {
        return 0;  /*regex compilation fails */
    }

    /* exec the regular expression */
    res = regexec(&reg, string, 0, NULL, 0);
    regfree(&reg);

    return res != REG_NOMATCH;
}

int str_substring(char* string, int start_ind, int end_ind, char* result) {
    int string_length = strlen(string);

    if (end_ind == -1) {
        end_ind = string_length;
    }

    /* Check for valid indices */
    if (string_length != 0 && (start_ind < 0 || start_ind >= string_length || end_ind < start_ind || end_ind > string_length)) {
        return FALSE;
    }

    /* Copy the substring to result */
    strncpy(result, string + start_ind, end_ind - start_ind);
    result[end_ind - start_ind] = '\0';

    return TRUE;
}



int remove_leading_and_trailing_whitespaces(char* string, char* result) {
    int start_ind;
    int end_ind;

    for (start_ind = 0; isspace((unsigned char)string[start_ind]); start_ind++) {
        /* Keep looping until not space */
    }

    for (end_ind = strlen(string) - 1; end_ind >= start_ind && isspace((unsigned char)string[end_ind]); end_ind--) {
        /* Keep looping until not space */
    }

    return str_substring(string, start_ind, end_ind + 1, result);
}


char* skip_separators(char* string, char* separator) {
    while (*string && strchr(separator, *string)) {
        string++;
    }
    return string;
}

char* get_last_separator(char* string, char* separator) {
    int separator_length = strlen(separator);
    char* current = string + strlen(string) - 1;

    while (strstr(current - separator_length, separator) == current && current - separator_length >= current) {
        current -= separator_length;
    }

    return current + 1;
}

int split_string_by_separator(char* string,char* separator, DoublyLinkedList** result_list, int max_splits){
    char* current;
    char item[4096];
    int number_of_splits =0;

    DoublyLinkedList* items_list = allocate_node_mem();

    do {
        number_of_splits++;
        string = skip_separators(string,separator);
        current = (strstr(string,separator));

        if(current == NULL){
            strcpy(item,string);
        } else{
            str_substring(string,0,current - string,item);
        }

        add_to_list(items_list, allocate_string(item));
        string = current;
    } while ((max_splits == -1 || number_of_splits < max_splits) && current != NULL && strlen(string) > 0);

    if(string != NULL && number_of_splits == max_splits){
        string = skip_separators(string,separator);
        current = get_last_separator(string,separator);

        if(string != current){
            str_substring(string, 0, current - string,item);
            add_to_list(items_list, allocate_string(item));
        }
    }

    *result_list = items_list;
    return 0;
}