
#include "byte_freq.h"


ByteFrequencies* ByteFrequencies_from_file(FILE* f) {
	ByteFrequencies *result = calloc(1, sizeof(ByteFrequencies));
	if (result == NULL) {
		perror("Could nota allocate enough memory.");
		exit(1);
	}
	long start_pos = ftell(f);
	if (start_pos == -1L) {
        perror("ftell failed");
        free(result);
        return NULL;
    }
	fseek(f, 0, SEEK_SET);
	if (fseek(f, 0, SEEK_SET) != 0) {
        perror("fseek failed");
        free(result);
        return NULL;
    }
	{
		int ch;
		while ((ch = fgetc(f)) != EOF) {
			if ((*result)[ch]+1 == 0) {
				for (size_t i=0; i<256; i++) {
					(*result)[i] = (*result)[i] >> 1;
				}
			}
			(*result)[ch]++;
		}
	}
	if (fseek(f, start_pos, SEEK_SET) != 0) {
        perror("fseek failed");
        free(result);
        return NULL;
    }
    return result;
}

void ByteFrequencies_free(ByteFrequencies** bf) {
	free(*bf);
	*bf = NULL;
}


