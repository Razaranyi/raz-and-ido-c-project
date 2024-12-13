#include <stdlib.h>
#include "address_encoded_pair.h"
#include "../utils/commons.h"

AddressEncodedPair* create_address_encoded_pair(unsigned long address, EncodedLine *encoded_line) {
    AddressEncodedPair *pair = (AddressEncodedPair*)malloc(sizeof(AddressEncodedPair));
    if (!pair) {
        exit(1);
    }
    pair->address = address;
    pair->encoded_line = encoded_line;
    return pair;
}

void free_address_encoded_pair(void *data) {
    AddressEncodedPair *pair = (AddressEncodedPair *)data;
    if (pair->encoded_line) {
        free_encoded_line(pair->encoded_line);
    }
    free(pair);
}
