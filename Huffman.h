///Author: Ji Kang
///Homework Assignment 2: Huffman Compression
///CSS 343, Prof. Bernstein
///Huffman tree class. 
///Class employs a min-heap (priority-queue) in order to build the tree. 
///Priority Queue's sole purpose to be a helper data structure to build the huffman tree. 
///The method transformHeap() is responsible for this action. The Huffman tree won't form unless this method is executed. 
///
///Class has encoding function (huff) and decoding function (puff). 
///*******************************
///ENCODING HUFFMAN TREE BUILDING
///Will take in a textfile and count the frequencies of each character ASCII value. It'll then make a 
///Frequency table of each character's count. From there, it will construct a priority queue (min heap) 
///of the characters with frequency counts. It will construct a huffman tree by popping the nodes (taking the minimum element) 
///constructing a huffman node and adding back to the tree. Once every node from the priority queue has been popped, a fully functional
///huffman tree will be created. It'll then make a table of coded symbols by traversing the tree and this will be used for the encoding process.
///*******************************
///DECODING HUFFMAN TREE BUILDING
///Will take in a huff file, read in the table of coded symbols, length, and then decode the file. 
#pragma once
#include <cstdlib>
#include "CodedSymbolTable.h"
#include "BitWriter.h"
#include "BitReader.h"
#include <string> 
#include <fstream>
#include <vector> 
#include <stdint.h>
#include <math.h>
#include "minHeap.h"
#include "Node.h"
#include <iostream>

class HuffmanTree {
public:
	HuffmanTree(std::string fileName, PrioHeap<unsigned int, unsigned char>* root, unsigned int);
	HuffmanTree() {}
	~HuffmanTree() { delete huffmanRoot; }
	void huff();
	void puff(std::string fileName);
    void test(Node<unsigned int, unsigned char>* node, std::string);
	void makeSymbolTable();
private:
	unsigned int totalChars;
	Node<unsigned int, unsigned char>* huffmanRoot;
	void privPuff(std::string fileName);
	void privHuff();
	void dumpTree();
	void privTransformHeap(PrioHeap<unsigned int, unsigned char>* root);
	std::string file; 
	void privDumpTree(int length, Node<unsigned int, unsigned char>*);
	void privMakeSymbolTable(Node<unsigned int, unsigned char>*, CodedSymbol);
	CodedSymbolTable symTable;
	unsigned int freqCount[256];
};
