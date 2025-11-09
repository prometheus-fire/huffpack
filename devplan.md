
# huffpack.
 A simple compression tool based on huffman encoding.

## 1. What is his this document?
This documents serves as a guide on how huffpack compression algorithm
works.

## 2. How huffpack works.

Huffpack is a command line tool for compressing files of any kind using
huffman encoding. 

Huffman encoding is a procedure that maps each byte of
a file to a more convenient binary representation: it allocates shorter
binary codes to more frequent bytes optimizing space. For example, in
case a file comprises only a's and b's, huffman encoding will map all
a's to bit 1 and all b's to bit 0 instead of giving them their space
inefficient ascii representation. 

The compression of a file will generate a new file with extension
.huff. The .huff file will have the following structure:

    file.hufff ::= [Header | lboffset | Compressed Bytes]

    [Compressed Bytes] ::= the raw compressed bit data from the original file
    
    [lboffset] ::= a number wich represents at which bit of the final byte of compressed bytes the decompression process should be ended.

    [Header] ::= [numbytes | level_order_bytes | shapebuffer]

    [numbytes] :: the number of distinct bytes used in file (represented by a single byte)

    [level_order_bytes] :: array of #numbytes bytes consisting of all the bytes used in the original file ordered by level order traversal of the huffman coding tree.

    [shapebuffer] :: a bitstring used to represent the shape of the huffman coding's tree. its size is 2*numbytes-1 bits.
