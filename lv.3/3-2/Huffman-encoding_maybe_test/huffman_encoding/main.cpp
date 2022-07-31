#include "Huffmancode.h"

int main() {
	CHuffmancode huffman;

	if (!huffman.LoadEncodeFile()) return -1;
	if (!huffman.SaveEncodeFile()) return -2;

	if (!huffman.LoadDecodeFIle()) return -3;
	if (!huffman.SaveDecodeFile()) return -4;
}


