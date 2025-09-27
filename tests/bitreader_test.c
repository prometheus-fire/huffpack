
#include <stdio.h>
#include <stdlib.h>
#include "bitfiles.h"

/*
 * This program writes argv[3] bits from argv[2] string into
 * argv[1] file; Used to test bitfiles.h
 */

int main(int argc, char *argv[]) {
	if (argc != 4) {
		puts("This program takes as arguments:\n");
		puts("dest_file data num_bits_to_write.");
		exit(1);
	}
	
	FILE* f = fopen(argv[1], "wb");
	if (f==NULL) {
		perror("could not open file.");
		exit(1);
	}
	
	BitWriter* bw = BitWriter_create(f);
	if (bw == NULL) exit(1);
	BitWriter_write(bw, argv[2], atoi(argv[3]));
	BitWriter_free(&bw);
	
	fclose(f);
	return 0;
}
