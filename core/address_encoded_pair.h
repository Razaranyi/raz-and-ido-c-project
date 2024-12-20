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

#endif
