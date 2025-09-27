/*
 * byte_freq is a module used to count the frequencies of bytes in
 * a certain file.
 */

#ifndef  BYTE_FREQ_H
#define BYTE_FREQ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/*
 * ByteFrequencies[char] ::= number of occurrences of char in a file.
 */
typedef uint64_t ByteFrequencies[CHAR_MAX];

/*
 * This function returns an approximate distribution of
 * byte frequencies in a file. This kind of distribution is used to
 * construct Huffman trees and is saved in the header of compressed
 * files.
*/ 
ByteFrequencies* ByteFrequencies_from_file(FILE* f);

void ByteFrequencies_free(ByteFrequencies** bf);

#endif
