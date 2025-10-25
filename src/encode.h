
/*
    This module is used to perform the compression of a
    file.
*/

#ifndef ENCODE_H
#define ENCODE_H

#include "bitstrings.h"
#include "bitfiles.h"
#include "hufftree.h"
#include "byte_freq.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
    Compresses file f into file out_path
    (f is opened as rb while out as wb).
    Returns 0 if compression was successful,
    1 otherwise.
*/
int compress(FILE *f, FILE *out_path);

#endif