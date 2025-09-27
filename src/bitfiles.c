
#include "bitfiles.h"

/*
 * Returns a pointer to a new bitwriter
 */
BitWriter* BitWriter_create(FILE* dest) {
	BitWriter* result = malloc(sizeof(BitWriter));
	if (result == NULL) {
		perror("BitWriter allocation failed.");
		return NULL;
	}
	result->dest = dest;
	result->curr_char = 0;
	result->curr_bit = 0; // ranges from 0 to CHAR_BIT-1
	return result;
}

void BitWriter_free(BitWriter** bw) {
	free(*bw);
	*bw = NULL;
}

/*
 * Write bitcount bits of data into bw.
 * Returns 0 if writing was successful, 1 if some error occurred.
 */
int BitWriter_write(BitWriter* bw, void *data, unsigned int bitcount) {
	unsigned char *src_bytes = (unsigned char *)data;
	for (unsigned int bit_index=0; bit_index<bitcount; bit_index++) {
		// get current bit read from data
		unsigned int byteindex = bit_index / CHAR_BIT;
		unsigned int bitshift = bit_index % CHAR_BIT;
		unsigned char curr_bit = (src_bytes[byteindex]>>bitshift) & 1;
		
		// write current bit into bw's curr_char buffer
		bw->curr_char = bw->curr_char | (curr_bit << bw->curr_bit);
		bw->curr_bit++;
		
		// when the bw->curr_char buffer fills, write it to bw->dest.
		if (bw->curr_bit == CHAR_BIT) {
			int res = fputc(bw->curr_char, bw->dest);
            if (res == EOF) {
                perror("Error encountered while writing file.");
                return 1;
            }
            // Reset the buffer for the next byte
            bw->curr_char = 0;
            bw->curr_bit = 0;
		}
	}
	return 0;
}
