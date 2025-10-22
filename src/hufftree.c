
#include "hufftree.h"

void HuffTree_free(HuffTree* hf) {
	if (hf == NULL) return;
	HuffTree_free(hf->left);
	HuffTree_free(hf->right);
	free(hf);
}

struct arr_el {
	uint64_t freq;
	HuffTree *node;
};

int compare(const void *a, const void *b) {
	struct arr_el *ael = (struct arr_el *)a;
	struct arr_el *bel = (struct arr_el *)b;
	if (ael->freq < bel->freq) return -1;
	if (ael->freq > bel->freq) return 1;
	return 0;
}

HuffTree* HuffTree_create(FILE* f) {
	// step 1: get byte frequencies.
	ByteFrequencies *bf = ByteFrequencies_from_file(f);
	if (bf == NULL) {
		perror("could not allocate enought memory.");
		return NULL;
	}
	
	// step 2: put all used byte frequencies in an array.
	// 2.1 : find out how much memory needed for frequencies.
	size_t array_size = 0;
	for (size_t i=0; i<=UCHAR_MAX; i++) {
		if ( (*bf)[i] > 0 ) array_size++;
	}
	// allocate array
	struct arr_el *array = calloc(array_size, sizeof(struct arr_el));
	if (array == NULL) {
		perror("could not allocate enought memory.");
		ByteFrequencies_free(&bf);
		return NULL;
	}
	// fill array
	{
		size_t curr_el = 0;
		for (size_t i=0; i<=UCHAR_MAX; i++) {
			if ( (*bf)[i] > 0 ) {
				HuffTree *new_node = malloc(sizeof(HuffTree));
				if (new_node == NULL) {
					perror("could not allocate enough memory.");
					ByteFrequencies_free(&bf);
					for (size_t j=0; j<curr_el; j++) {
						HuffTree_free(array[i].node);
					}
					free(array);
					return NULL;
				}
				new_node->val   = (unsigned char)i;
				new_node->left  = NULL;
				new_node->right = NULL;
				struct arr_el *curr = array + curr_el;
				curr->node = new_node;
				curr->freq = (*bf)[i];
				curr_el++;
			}
		}
	}
	// we can free byte frequencies as we dont need them anymore.
	ByteFrequencies_free(&bf);
	
	// step 3: sort array in non decreasing order based on frequencies.
	qsort((void *)array, array_size, sizeof(struct arr_el), compare);
	
	// step 4: generate the hufftree
	for (size_t i=0; i<array_size-1; i++) {
		// combine the two least frequent elements into a node
		HuffTree *new_node = malloc(sizeof(HuffTree));
		if (new_node == NULL) {
			perror("couldn't allcate enough memory.");
			for (size_t j=i; j<array_size; j++) {
				HuffTree_free(array[j].node);
			}
			free(array);
			return NULL;
		}
		new_node->left = array[i].node;
		new_node->right = array[i+1].node;
		new_node->val = '\0';
		// save the node into array[i+1].node;
		array[i+1].freq += array[i].freq;
		array[i+1].node = new_node;
		// bubblesort array[i+1] with the rest of the array
		size_t j=i+1;
		while (j<array_size-1 && array[j].freq > array[j+1].freq) {
			struct arr_el temp = array[j];
			array[j] = array[j+1];
			array[j+1] = temp;
			j++;
		}
	}
	// the resulting hufftree is now in the last element of the array
	HuffTree *result = array[array_size-1].node;
	free(array);
	return result;
}
