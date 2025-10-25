
/*
    This header is used for writing bitstrings.
    A bitstring is just a character array written
    bit by bit. 

    Bits in a bitstring's data field are ordered as such

    most significant
    |       |---------least significant   
    v       v
     byte 0    byte 1   ...
    *--------*--------*--------*
    |76543210|......98| ...    |
    *--------*--------*--------*
*/

#ifndef BITSTRINGS_H
#define BITSTRINGS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

typedef struct {
    uint16_t length;
    unsigned char *data;
} BitString;

/*
    Takes a null terminated string s consisting of 
    '0' and '1' and uses it to initialize bs in bitstring
    order.
    Returns:
    0 if initialization was successful,
    1 if s contains characters different from '0' '1' '\0',
    2 if memory allocation for data went wrong
*/
int Bitstring_create(BitString *bs, const char *s);

/*
    Frees bs->data and sets length to 0.
*/
void Bitstring_free(BitString *bs);

#endif