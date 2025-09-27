
#include <stdio.h>
#include <stdlib.h>
#include "byte_freq.h"

/*
 * This program takes a file as argv[1] and displays the frequencies
 * of each one of its bytes.
 */
 
int main(int argc, char *argv[]) {
	if (argc != 2) {
		puts("Filename expected as argument.\n");
		return 0;
	}
	FILE *f = fopen(argv[1], "rb");
	if (f == NULL) {
		perror("Could not open file.");
		exit(1);
	}
	ByteFrequencies* bf = ByteFrequencies_from_file(f);
	if (bf == NULL) {
		exit(1);
	}
	for (unsigned int i=0; i<CHAR_MAX; i++) {
		if ( (*bf)[i] != 0 ) {
			printf("char : %c, frequency: %ld\n", (char)i, (*bf)[i]);
		}
	}
	ByteFrequencies_free(&bf);
	fclose(f);
	return 0;
}
