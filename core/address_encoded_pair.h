#ifndef ADDRESS_ENCODED_PAIR_H
#define ADDRESS_ENCODED_PAIR_H

#include "encoded_line.h"

typedef struct {
    unsigned long address;
    EncodedLine *encoded_line;
} AddressEncodedPair;

AddressEncodedPair* create_address_encoded_pair(unsigned long address, EncodedLine *encoded_line);
void free_address_encoded_pair(void *data);
void print_address_encode_list(DoublyLinkedList *list);
/*get encoded line, combie all the bits field end return unsigned int with it*/
unsigned int get_encoded_line_as_binary(EncodedLine *encoded_line);
#endif
