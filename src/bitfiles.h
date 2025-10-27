/*
 * Bitfiles is a module used to read or write files bit by bit.
 * This is used in the compression phase to write the raw compressed
 * data to a file and in the decompression phase to read the raw
 * decompressed data.
 */
 
#ifndef BITFILES_H
#define BITFILES_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * BitWriter is a struct used to write a file bit by bit.
 * 		dest: file pointer to be written.
 * 		curr_bit: curr_bit being written into curr_char 
 * 				  (ranges 0..CHAR_BIT-1)
 * 		curr_char: curr_char to be written to dest.
 */
typedef struct {
	FILE* dest;
	unsigned char curr_bit;
	unsigned char curr_char;
} BitWriter;

/*
* BitReader is the equivalent of BitWriter but for reading from files.
*/
typedef BitWriter BitReader;


/*
 * Returns a pointer to a new bitwriter. Returns NULL if some error
 * occurred.
 */
BitWriter* BitWriter_create(FILE* dest);

/*
 * WARNING: use this function BEFORE closing dest
 */
void BitWriter_free(BitWriter** bw);

/*
 * Write bitcount bits of data into bw.
 * Returns 0 if writing was successful, 1 if some error occurred.
 */
int BitWriter_write(BitWriter* bw, void *data, unsigned int bitcount);

/*
	Returns a pointer to a new BitReader.
	Returns NULL if some error occurred.
*/
BitReader* BitReader_create(FILE* src);

/*
	Frees a bitreader pointer and sets it to NULL
*/
void BitReader_free(BitReader** br);

/*
	Reads a bit from the current file location.
	Returns 0 or 1 depending on the bit read if reading was successful,
	EOF if file ended.
*/
int BitReader_read(BitReader* br);


#endif
