#include <iostream> 
#include <string>
#include <fstream> 
#include "Huffman.h"
#include "minHeap.h"
using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cerr << "Invalid file input" << endl;
		return EXIT_FAILURE;
	}
	HuffmanTree huffTree;
	string fileName = argv[1];
	string extension = fileName.substr(fileName.find(".") + 1);
	if (extension != "huff") {
		return EXIT_FAILURE;
	}
	huffTree.puff(fileName);
	return 0;
}
