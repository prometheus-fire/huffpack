#include "bitstrings.h"

int Bitstring_create(BitString *bs, const char *s)
{
    size_t len = strlen(s);
    if (len == 0) {
        bs->data = NULL;
        bs->length = 0;
        return 0;
    }

    size_t bytelen = len / 8 + (len%8 == 0);
    bs->data = calloc(bytelen, sizeof(unsigned char));
    if (bs->data == NULL) {
        perror("could not allocate enough memory for bitstring");
        return 2;
    }

    bs->length = (uint16_t)len;

    for (uint16_t i=0; i<(uint16_t)len; i++) {
        unsigned char *curr_byte = bs->data + (i / CHAR_BIT);
        unsigned char curr_bit = 1 << (i % CHAR_BIT);
        if (s[i] == '0') curr_bit = 0;
        else if (s[i] != '1') {
            free(bs->data);
            bs->length = 0;
            return 1;
        }
        *curr_byte = *curr_byte | curr_bit;
    }

    return 0;
}

void Bitstring_free(BitString *bs)
{
    free(bs->data);
    bs->length = 0;
}
