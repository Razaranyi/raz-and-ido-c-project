#ifndef ADDRESS_ENCODED_PAIR_H
#define ADDRESS_ENCODED_PAIR_H

#include "encoded_line.h"
/*struct that holds pairs of address & its encoded line*/
typedef struct {
    unsigned long address;
    EncodedLine *encoded_line;
} AddressEncodedPair;
/*Return new node of pair*/
AddressEncodedPair* create_address_encoded_pair(unsigned long address, EncodedLine *encoded_line);
/*free node of address encoded line pair*/
void free_address_encoded_pair(void *data);
/*get encoded line, combine all the bits field end return unsigned int with it*/
unsigned int get_encoded_line_as_binary(EncodedLine *encoded_line);
#endif
