#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cwalk.h> 

#include "encode.h"

int main(int argc, char *argv[]) {

    char outdir_buf[FILENAME_MAX] = "./"; 

    char *outdir = outdir_buf; 
    int outdir_specified = 0;
    const char ext[] = ".huff";

    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        FILE *help = fopen("./misc/huffpack_help.txt", "rb");
        if (help == NULL) {
            puts("missing help file");
            return 1;
        }
        int ch;
        while ((ch = fgetc(help)) != EOF) {
            putchar(ch);
        }
        fclose(help);
        return 0;
    }

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-o") == 0) {
            i++;
            if (i == argc) {
                puts("dir name must be specified for optional parameter -o");
                exit(1);
            }
            if (outdir_specified == 1) {
                puts("output directory specified more than once");
                exit(1);
            }
            outdir_specified = 1;

            strncpy(outdir_buf, argv[i], sizeof(outdir_buf) - 1);
            outdir_buf[sizeof(outdir_buf) - 1] = '\0'; 

            cwk_path_normalize(outdir_buf, outdir_buf, sizeof(outdir_buf));
            
            continue;
        }

        FILE *r = fopen(argv[i], "rb");
        if (r == NULL) {
            perror("could not open file");
            continue;
        }


        const char *basename;
        size_t basename_len;
        char basename_nullterm[FILENAME_MAX];
        char joined_path[FILENAME_MAX];
        char final_out_path[FILENAME_MAX];

        cwk_path_get_basename(argv[i], &basename, &basename_len);

        if (basename_len >= sizeof(basename_nullterm)) {
            fprintf(stderr, "ERRORE: filename is too long %s\n", argv[i]);
            fclose(r);
            continue;
        }
        strncpy(basename_nullterm, basename, basename_len);
        basename_nullterm[basename_len] = '\0';


        cwk_path_join(outdir, basename_nullterm, joined_path, sizeof(joined_path));
        cwk_path_change_extension(joined_path, ext, final_out_path, sizeof(final_out_path));

        FILE *w = fopen(final_out_path, "wb");

        if (w == NULL) {
            fprintf(stderr, "ERROR: could not create file %s\n", final_out_path);
            continue;
        } else if (compress(r,w) == 0) {
            printf("Compressed %s in %s\n", argv[i], final_out_path);
        } else {
            fprintf(stderr, "ERROR: compression of %s into %s failed\n", argv[i], final_out_path);
		}

        fclose(r);
        fclose(w);
    }

    return 0;
}