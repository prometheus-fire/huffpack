
#include <stdio.h>
#include <stdlib.h>
#include "bitstrings.h"

int main() {

	unsigned char test = 'A';
	printf("test character : %c %d\n", test, test);
	
	char str[CHAR_BIT+1];
	for (size_t i=0; i<CHAR_BIT; i++) {
		unsigned char ti = 1 << i;
		int curr_bit = (test & ti) >> i;
		if (curr_bit == 1) {
			str[i] = '1';
		} else if (curr_bit == 0){
			str[i] = '0';
		} else {
			exit(1);
		}
	}
	str[CHAR_BIT] = '\0';

	printf("string for test character : %s\n", str);

	BitString result;
	Bitstring_create(&result, str);

	printf("Result : %c\n", result.data[0]);

	Bitstring_free(&result);
	return 0;
}