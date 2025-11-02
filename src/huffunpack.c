
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decode.h"

int main(int argc, char *argv[]) {

    if (argc < 2) return 0;

    for (int i=1; i<argc; i++) {
        // check if filename ends with .huff
        char extension[] = ".huff";
        int ext_len = strlen(extension);
        
        int curr_len = strlen(argv[i]);
        if (curr_len <= ext_len) {
            printf("ERROR with %s: incompatible filename.\n", argv[i]);
            continue;
        }
        char *curr = malloc(curr_len+1);
        strcpy(curr, argv[i]);

        int cmp = strcmp(curr+curr_len-ext_len, extension);
        if (cmp != 0) {
            printf("ERROR with %s : file extension must be %s.\n", curr, extension);
        }
        else {
            curr[curr_len-ext_len] = '\0';
            FILE *in = fopen(argv[i], "rb");
            FILE *out = fopen(curr, "wb");
            int result = decode(in, out);
            if (result != 0) {
                printf("Warning: something went wrong while decoding %s\n",
                argv[i]);
            }
            fclose(in);
            fclose(out);
        }

        free(curr);
    }

    return 0;
}