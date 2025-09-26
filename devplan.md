
# huffpack.
 A simple compression tool based on huffman encoding.

## 1. What is his this document?
This documents serves as a guide for the future developement of
huffpack. The aim of the guidelines here provided is to draw a sky
of fixed stars in reference to which the project will navigate. Some
changes might be made in the progress, but the main direction will
remain still.

## 2. How will huffpack work.

Huffpack is a command line tool for compressing files of any kind using
huffman encoding. 

Huffman encoding is a procedure that maps each byte of
a file to a more convenient binary representation: it allocates shorter
binary codes to more frequent bytes optimizing space. For example, in
case a file comprises only a's and b's, huffman encoding will map all
a's to bit 1 and all b's to bit 0 instead of giving them their space
inefficient ascii representation. 

For the first version it will be possible to encode
files one by one, but in the future a cool feature could be to extend
this functionality to whole directories (in a similar way to what tar
and zip do).

The compression of a file will generate a new file with the extension
.hfp. The .hfp file will have the following structure:

file.hpf ::= [Header | Compressed Bytes]

[Compressed Bytes] ::= the raw compressed bit data from the original
file

[Header] ::= [lboffset | byte_frequencies]

[lboffset] ::= a number wich represents at which bit of the final byte
of compressed bytes the decompression process should be ended.

[byte_frequencies] :: a list of byte frequencies starting from the least
to the most frequent byte used in the original file.


## 3. User interface

Huffpack will use for its first version a command line interface
with limited options. In the future new features may be added.
The user interface will work roughly as this:

huffpack file_path (optional)[-o dest_file_path (default: file_path)]

If file_path has the .hfp file extension decompression will be exectuted
producing file dest_filepath (by default the produced file has the same
name of the source file path but without the .hfp extension). 
Compression into .hfp is otherwise performed by adding .hpf to the
extension of the original file.



