#include "commons.h"

/*get filename and suffix and add the suffix to the file to write fullpath*/
char* add_end_to_filename(const char* filename, const char* suffix) {
    size_t filename_len = strlen(filename);
    size_t suffix_len = strlen(suffix);
    char* new_filename = (char*)malloc(filename_len + suffix_len + 1);
    if (new_filename == NULL) 
    {
        perror("Memory allocation failed");
        return NULL;
    }

    strcpy(new_filename, filename);
    strcat(new_filename, suffix);

    return new_filename;
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

char* allocate_string(char* input) {
    char* copy;
    if (input == NULL) {
        return NULL;
    }
    copy = (char*)malloc(strlen(input) + 1);
    if (copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    strcpy(copy, input);
    return copy;
}

int* allocate_int(int integer){
    int* ptr = malloc(sizeof (int));
    *ptr = integer;
    return ptr;
}
int is_all_whitespace(char *str) {
    while (*str) {
        if (!isspace(*str)) {
            return FALSE;
        }
        str++;
    }
    return TRUE;
}

int is_string_begin_with_substring(char* string, char* substring){
    return strncmp(string, substring, strlen(substring)) == 0;
}



int is_label( char* line) {
    /* Pattern to match a label: a valid label name ending with ':' */
    char* pattern = "^[a-zA-Z][a-zA-Z0-9]*:.*";
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
    memmove(result, string + start_ind, end_ind - start_ind);
    result[end_ind - start_ind] = '\0';

    return TRUE;
}


int remove_leading_and_trailing_whitespaces(char* string, char* result) {
    char temp[4096]; /* Large enough buffer */
    int start_ind, end_ind;
    int string_len = strlen(string);

    /* Find start_ind */
    for (start_ind = 0; start_ind < string_len && isspace((unsigned char)string[start_ind]); start_ind++) {}
    /* If entire string is spaces */
    if (string[start_ind] == '\0') {
        strcpy(result, "");
        return TRUE;
    }

    /* Find end_ind */
    for (end_ind = string_len - 1; end_ind >= start_ind && isspace((unsigned char)string[end_ind]); end_ind--) {}

    /* copy the substring to temp */
    str_substring(string, start_ind, end_ind + 1, temp);

    /* Copy from temp to result */
    strcpy(result, temp);
    return TRUE;
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

int split_string_by_separator(char* string, char* separator, DoublyLinkedList** result_list, int max_splits) {
    char* current;
    char item[4096];
    int number_of_splits = 0;
    DoublyLinkedList* items_list;


    if (!string || !separator || strlen(separator) == 0) {
        fprintf(stderr, "Invalid arguments to split_string_by_separator.\n");
        return -1;
    }


    items_list = allocate_node_mem();
    if (!items_list) {
        fprintf(stderr, "Failed to allocate memory for result list.\n");
        return -1;
    }

    do {
        number_of_splits++;
        string = skip_separators(string, separator);
        current = strstr(string, separator);

        if (current == NULL) {
            strncpy(item, string, sizeof(item) - 1);
            item[sizeof(item) - 1] = '\0';
        } else {
            str_substring(string, 0, current - string, item);
        }

        add_to_list(items_list, allocate_string(item));
        string = current ? current + strlen(separator) : NULL;
    } while ((max_splits == -1 || number_of_splits < max_splits) && current != NULL);

    *result_list = items_list;
    return 0;
}

/*convert address from usigned long to char * and add 0 in the begging for the files format*/
char * fix_address(unsigned long address)
{
    char *str, *new_str;
    int i, num_zeros;
    size_t length;
    str = (char *)malloc(21); /*the biggest unsigend long can be*/
    sprintf(str, "%lu", address);
    length = strlen(str);
    num_zeros = ADDRESS_LEN - length;
    new_str = (char *)malloc(length + num_zeros + 1);
    new_str[0] = '\0';
    for(i=0; i < num_zeros; i++) /*to fix the format*/
        strcat(new_str, "0");
    strcat(new_str, str);
    free(str);
    return new_str;
}


