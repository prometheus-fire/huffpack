
#ifndef HUFFTREE_H
#define HUFFTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "byte_freq.h"

typedef struct hftree {
	struct hftree *left;
	struct hftree *right;
	unsigned char val;
} HuffTree;

HuffTree* HuffTree_create(FILE* f);

void HuffTree_free(HuffTree* hf);

#endif
