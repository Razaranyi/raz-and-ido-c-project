#include <stdlib.h>
#include <stdio.h>
#include "address_encoded_pair.h"
unsigned int get_encoded_line_as_binary(EncodedLine *encoded_line) {
    unsigned int encoded_value = 0;

    /* Shift and combine all the fields according to the required bit layout */
    encoded_value |= (encoded_line->opcode & 0x3F) << 18;               /* 6 bits for opcode */
    encoded_value |= (encoded_line->src_addressing & 0x3) << 16;        /* 2 bits for source addressing */
    encoded_value |= (encoded_line->src_reg & 0x7) << 13;               /* 3 bits for source register */
    encoded_value |= (encoded_line->dst_addressing & 0x3) << 11;       /* 2 bits for destination addressing */
    encoded_value |= (encoded_line->dst_reg & 0x7) << 8;                /* 3 bits for destination register */
    encoded_value |= (encoded_line->funct & 0x1F) << 3;                 /* 5 bits for function */
    encoded_value |= (encoded_line->are & 0x7);                        /* 3 bits for A, R, E bits */


    return encoded_value;
}

AddressEncodedPair* create_address_encoded_pair(unsigned long address, EncodedLine *encoded_line) {
    AddressEncodedPair *pair = (AddressEncodedPair*)malloc(sizeof(AddressEncodedPair));
    if (!pair) {
        exit(1);
    }
    pair->address = address;
    pair->encoded_line = encoded_line;
    return pair;
}

void free_address_encoded_pair(void* data) {
    AddressEncodedPair* pair = (AddressEncodedPair*)data;
    if (pair != NULL) {
        if (pair->encoded_line != NULL) {
            free_encoded_line(pair->encoded_line);
            pair->encoded_line = NULL;
        }
        free(pair);
    }
}


