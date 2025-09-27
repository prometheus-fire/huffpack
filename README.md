# huffpack
A simple compression tool based on huffman encoding.

## Developement state
- Created bitfiles.h (a header for writing a file bit by bit) and written a 
test for it in main.c inside /src folder.
- To run it, build everything with make and run ./test. test takes as
arguments the destination file path to where to write the bits, a string
to take data from and the number of bits to write into the destination file.
(example: ./tests out.txt test 16 writes bytes te into out.txt)

