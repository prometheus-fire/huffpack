
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"



int main(int argc, char *argv[]) {

	int outdir_specified = 0;
	char *outdir = "./";
	const char ext[] = ".huff";

	if (argc == 2 && strcmp(argv[1], "--help")==0) {
		FILE *help = fopen("./misc/huffpack_help.txt", "rb");
		if (help == NULL) {
			puts("missing help file");
			return 1;
		}
		int ch;
		while ((ch=fgetc(help)) != EOF) {
			putchar(ch);
		}
		fclose(help);
		return 0;
	}
	
	for (int i=1; i<argc; i++) {

		if (strcmp(argv[i], "-o")==0) {
			i++;
			if (i==argc) {
				puts("dir name must be specified for optional parameter -o");
				exit(1);
			}
			if (outdir_specified==1) {
				puts("output directory specified more than once");
				exit(1);
			}
			outdir_specified = 1;
			outdir = argv[i];
			continue;
		}

		FILE *r = fopen(argv[i], "rb");
		if (r == NULL) {
			perror("could not open file");
			exit(1);
		}

		char *out_path = malloc(
			strlen(outdir)+
			strlen(argv[i])+ 
			strlen(ext)+ 
			1
		);
		if (out_path == NULL) {
			perror("could not allocate enough memory");
			exit(1);
		}
		strcpy(out_path, outdir);
		strcat(out_path, argv[i]);
		strcat(out_path, ext);

		FILE *w = fopen(out_path, "wb");
		free(out_path);
		if (w == NULL) {
			perror("could not open file");
			exit(1);
		}

		compress(r,w);

		fclose(r);
		fclose(w);
	}

	return 0;
}