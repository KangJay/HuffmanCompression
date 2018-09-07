#include <iostream> 
#include <fstream> 
#include "Huffman.h"
#include "minHeap.h"
using namespace std; 
int main(int argc, char* argv[]) {
	if (argc != 2){
		cerr << "Invalid input." << endl; 
		return EXIT_FAILURE;
	}
	//cout << argv[1] << endl;
	////HuffmanTree<int, char> minHeap(argv[1]);
	//HuffmanTree<int, char> minHeap(argv[1]);
	//minHeap.huff();
	//minHeap.test();
	PrioHeap<unsigned int, unsigned char> minHeap(argv[1]);
	//PrioHeap<int, char> minHeap("comsn10.txt");
	HuffmanTree huffTree(argv[1], &minHeap, minHeap.getTotal());
	huffTree.huff();
	return 0; 
}
