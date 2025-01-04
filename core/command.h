#ifndef commandS_H
#define commandS_H

#include "../utils/boolean.h"
#include "doubly_linked_list.h"
#include "encoded_line.h"
#include "address_encoded_pair.h"

/* addressing modes */
#define ADDRESSING_IMMEDIATE 0
#define ADDRESSING_DIRECT    1
#define ADDRESSING_RELATIVE  2
#define ADDRESSING_REGISTER  3

/** Command structure including all command (not instruction) relevant fields*/
typedef struct {
    char* command_name;
    int opcode;
    int funct;  /* Set to -1 when not used */
    int number_of_operands;

    /* Allowed addressing modes for source and destination operands */
    int allowed_src_addressing_modes[4]; /* 0 -> A = 1, 1 -> (external)E = 1 || R = 1, 2 -> A = 1, */
    int allowed_dst_addressing_modes[4];
} Command;

/** Global command set */
extern DoublyLinkedList* command_list;


/** Initializes the command set */
void initialize_command_set();

/** free command set */
void free_command_set();

/** Finds a command by name */
Command* find_command(char* name);

/** Cheks if given name is an command name */
int is_command_name(char* name);

/** Checks if a given addressing mode is allowed for the source operand */
int is_src_addressing_mode_allowed(Command* command, int mode);

/** Checks if a given addressing mode is allowed for the destination operand */
int is_dst_addressing_mode_allowed(Command* command, int mode);
/**Handle the command operands including adding it to the encoded line*/
int handle_command_operands(Command *command,
                            DoublyLinkedList *operands,
                            DoublyLinkedList  *address_encoded_line_pair_list,
                            EncodedLine *encoded_line,
                            int line_index,
                            int *error_found,
                            unsigned long *IC
);

/**Parses command operands including validation checks*/
void parse_command_operands(
        Command *command,
        char *operands_str,
        DoublyLinkedList **operands,
        int *error_found,
        int line_index
);
/**Parsing command and adding it's details to the encoded line, then sending to additional parsing of the operands*/
void process_command_line(
        char *line_content,
        char *label,
        int line_index,
        char *command_token,
        DoublyLinkedList *symbol_table,
        DoublyLinkedList *address_encoded_line_pair_list,
        unsigned long *IC,
        int *error_found
);

#endif
