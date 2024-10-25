#include <stdio.h>
#include "utils/commons.h"
#include "utils/logger.h"
#include "utils/boolean.h"
#include "core/instruction.h"
#include "core/doubly_linked_list.h"


int is_args_valid(int argc);



int main(int argc, char** argv) {
    int i;
    if (!is_args_valid(argc)) {
        error("Invalid exec command!.\n try: ./assembler <file1>.as <file2>.ac ...", 0);
        exit(1);
    }

    initialize_instruction_set();
    if (is_instruction_name("mov")) {
        printf("Success mov is an instruction\n");
    }
    if(!is_instruction_name("MOV")){
        printf("Success. MOV is not an instruction!\n");
    }

        for (i = 1; i < argc; i++) {
            /* compile(argv[i]); */
        }
    return 0;
}


int is_args_valid(int argc){
    return argc > 1;
}