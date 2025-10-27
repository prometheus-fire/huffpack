
#include <stdio.h>
#include "bitfiles.h"

int main(int argc, char *argv[]) {

    if (argc != 2) return 0;

    FILE *in = fopen(argv[1], "rb");
    BitReader *br = BitReader_create(in);
    int bit;
    while ((bit = BitReader_read(br)) != EOF) {
        printf("%c", bit==0 ? '0' : '1');
    }
    putchar('\n');
    BitReader_free(&br);
    fclose(in);

    return 0;
}