#include <stdio.h>
#include "commons.h"
#include "logger.h"
#include "utils/boolean.h"
#include "instructions.h"
#include "core/doubly_linked_list.h"


int is_args_valid(int argc);

int main(int argc, char** argv[]) {
    int i;
    if(!is_args_valid(argc)){
        error("Invalid exec command!.\n try: ./assembler <file1>.as <file2>.ac ...",0);
        exit(1);
    }
    initialize_instruction_set();
    for (i = 1, i<argc ,i++){
        snprintf(formatted_message, sizeof(formatted_message), "Starting compiling file number %d ...", i);
        debug(formatted_message, __LINE__);
        compile(argv[i]);
    }
    free_list(&instruction_list,free_instruction_data);
    return 0;
}

int is_args_valid(int argc){
    return argc > 1;
}