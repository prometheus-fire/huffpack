
#include <stdio.h>
#include <stdlib.h>
#include "hufftree.h"


void visit(HuffTree* ht) {
	int isleaf = 1;
	if (ht->left) {
		putchar('0');
		isleaf = 0;
		visit(ht->left);
	} else { putchar('1'); }
	if (ht->right) {
		putchar('0');
		isleaf = 0;
		visit(ht->right);
	} else { putchar('1'); }
	if (isleaf == 1) {
		putchar(ht->val);
	}
}


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
	HuffTree *ht = HuffTree_create(f);
	fclose(f);

	visit(ht);
	putchar('\n');

	HuffTree_free(ht);
	return 0;
}
