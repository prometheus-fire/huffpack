# huffpack
A simple cross platform compression utility based on huffman encoding
written in c. It currently supports files up to ~2GiB size.

## Interface 
This utility consists of two commands, huffpack and huffunpack,
that can be used to compress and decompress files. 

huffpack example usage (see more with huffpack --help):

    huffpack file
    >> ./file.huff

    huffpack file1 -o dir file2 file3
    >> ./file1.huff
    >> dir/file2.huff
    >> dir/file3.huff

Huffunpack takes as input path.huff files and decompresses them into a new file named path (see more with huffunpack --help):

    huffpack file.huff
    >> ./file

    huffpack file1.huff -o dir file2.huff file3.huff
    >> ./file1
    >> dir/file2
    >> dir/file3

## Compilation

To compile huffpack and huffunpack just run the Makefile (remember to change CC=your_fav_compiler before running).

## How it works

If you want to know how the compression and decompression processes work you can read the file devplan.md