#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cwalk.h>

#include "decode.h"

#define EXT ".huff"

int main(int argc, char *argv[]) {

    // output directory, by default is .
    char *outdir = ".";

    // manage help option
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        FILE *help = fopen("./misc/huffunpack_help.txt", "rb");
        if (help == NULL) {
            puts("missing help file");
            return -1;
        }
        int ch;
        while ((ch = fgetc(help)) != EOF) {
            putchar(ch);
        }
        fclose(help);
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        // if -o argument is passed, the next files taken as input
        // will have the new directory specified as input
        if (strcmp(argv[i], "-o") == 0) {
            i++;
            if (i == argc) {
                puts("dir name must be specified for optional parameter -o");
                exit(1);
            }
            outdir = argv[i];
            cwk_path_normalize(outdir, outdir, strlen(outdir)+1);
            continue;
        }

        // check if input file has EXT extension,
        {
            const char *input_ext;
            size_t extlen;
            bool found_ext = cwk_path_get_extension(argv[i], &input_ext, &extlen);
            if (!found_ext || strcmp(input_ext, EXT) != 0) {
                fprintf(stderr, "ERROR: %s file extension is not %s, ignored\n", argv[i], EXT);
                continue;
            }
        }

        // open input file
        FILE *r = fopen(argv[i], "rb");
        if (r == NULL) {
            perror("could not open file");
            continue;
        }

        // get basename of input file
        const char *basename;
        cwk_path_get_basename(argv[i], &basename, NULL);

        char *new_basename = malloc(strlen(basename)+1);
        if (new_basename == NULL) {
            perror("could not allocate enough memory for filename");
            fclose(r);
            exit(1);
        }
        strcpy(new_basename, basename);

        // remove .huff extension from basename
        {
            size_t extension_dot_pos = strlen(new_basename) - strlen(EXT);
            new_basename[extension_dot_pos] = '\0';
        }

        // generate final out path
        size_t final_out_path_size = (strlen(outdir)+strlen(new_basename)) * 2;
        char *final_out_path = malloc(final_out_path_size);
        if (final_out_path == NULL) {
            perror("could not allocate enough memory for filename");
            fclose(r);
            free(new_basename);
            exit(1);
        }
        cwk_path_join(outdir, new_basename, final_out_path, final_out_path_size);
        free(new_basename);

        // open the resulting file
        FILE *w = fopen(final_out_path, "wb");

        // decompress input file into output file
        if (w == NULL) {
            fprintf(stderr, "ERROR: could not create file %s\n", final_out_path);
            free(final_out_path);
            continue;
        } else if (decode(r,w) == 0) {
            printf("Deompressed %s in %s\n", argv[i], final_out_path);
        } else {
            fprintf(stderr, "ERROR: decompression of %s into %s failed\n", argv[i], final_out_path);
		}

        free(final_out_path);
        fclose(r);
        fclose(w);
    }

    return 0;
}