
#include <stdio.h>
#include <stdlib.h>
#include "encode.h"

int main(int argc, char *argv[]) {
	if (argc != 2) return 0;
	FILE *r = fopen(argv[1], "rb");
	if (r == NULL) exit(1);
	FILE *w = fopen("output", "wb");

	compress(r,w);

	fclose(r);
	fclose(w);

	return 0;
}