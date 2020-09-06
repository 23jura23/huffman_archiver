# Huffman archiver

A small tool to archive and extract files using [huffman coding](https://en.wikipedia.org/wiki/Huffman_coding)

Briefly, the archiving process:
1. Reads file
2. Analyses symbols frequencies
3. Reencodes symbols in the most economic way
4. Writes header with frequencies table to output file
5. Writes encoded symbols to output files

And extracting process:
1. Reads file
2. Parses the frequencies table from header
3. Decode symbols
4. Writes to output files

## Usage

```
Usage: huffman (-c | -u) (-f | --file) input (-o | --output) output

-c             for archiving
-u             for extracting
-f or --file   file to archive/extract
-o or --output file to write result
```
