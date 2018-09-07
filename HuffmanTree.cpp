///Author: Ji Kang
///Homework Assignment 2: Huffman Compression
///CSS 343, Prof. Bernstein
///Huffman tree class. 
///Class employs a min-heap (priority-queue) in order to build the tree. 
///Priority Queue's sole purpose to be a helper data structure to build the huffman tree. 
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

#include "Huffman.h"
#include <iostream> 

using namespace std; 

HuffmanTree::HuffmanTree(std::string fileName, PrioHeap<unsigned int, unsigned char>* root, unsigned int count)
{
	file = fileName; 
	totalChars = count; 
	privTransformHeap(root);
}

/*
Precondition: None
Postcondition: None
Private method that'll convert the heap into a huffman tree. 
See documentation at top for details
*/
void HuffmanTree::privTransformHeap(PrioHeap<unsigned int, unsigned char>* root)
{
	for (int i = 0; i < 256; i++) {
		freqCount[i] = root->freqCount[i];
	} 
	int count = root->getSize();
	int totalPrio;
	Node<unsigned int, unsigned char>* n1;
	Node<unsigned int, unsigned char>* n2;
	
	while (count != 1) {
		n1 = root->pop();
		if (count - 1 == 0) {
			n2 = nullptr;
		}
		else {
			n2 = root->pop();
		}
		totalPrio = n1->getPriority() + n2->getPriority();
		root->push(n1, n2, totalPrio);
		count--; 
	}
	huffmanRoot = root->pop(); //Last element in the heap is the root to the huffman tree. 
	//dumpTree();
}

/*
Precondition: None
Postcondition: None
Method recursively passes a coded symbol object as well as the huffman tree node
representing the symbol's current place in recursion. 
It will push 0's or 1's based on left and right traversal.
If it hits a leaf node, the recursion will stop for that given recursion
and save the data given so far. 
*/
void HuffmanTree::privMakeSymbolTable(Node<unsigned int, unsigned char>* node, CodedSymbol symbol)
{
	if (!node->getLeft() && !node->getRight()) {
		unsigned char character = node->getData();
		symbol.fillBit(); //Fills remainders with 0's
		symTable.setCharCode(character, symbol);
	}
	else {
		CodedSymbol leftChild = symbol;
		CodedSymbol rightChild = symbol;
		leftChild.pushBit(0);
		rightChild.pushBit(1);
		privMakeSymbolTable(node->getLeft(), leftChild);
		privMakeSymbolTable(node->getRight(), rightChild);
	}
}

//Public interface method for making the table. 
void HuffmanTree::makeSymbolTable() 
{
	CodedSymbol emptySymbol; 
	privMakeSymbolTable(huffmanRoot, emptySymbol);
	//symTable.dump(freqCount);
}

//Public interface method for encoding
void HuffmanTree::huff()
{
	privHuff();
}

/*
Precondition: None
Postcondition: None
Encoding method. It will use the name of the input textfile used but replace the extension. 
This file will be the output write file. We will need to make a second pass over 
the original text file to begin the encoding. 
First, the method will write out our coded symbols table so our decoder in the future will know
how to decode it. 
It will send an unsigned char representing the number of bits (or bit length) of a given coded symbol. 
Based on that number, it will write ceil(bitLength / 8) number of bytes and flush the remaining space if needed. 
--> This means characters with no frequency will have no corresponding bytes written afterward. 
Once all ASCII characters (0 to 255 inclusive) are written, we will need to byte manipulate an unsigned int representing
the total number of characters in our given text file. Once that is completed, we will write out to file once more. 
Now the actual text encoding begins. Given our coded symbols table, we wil write out the corresponding bits based on 
the binary code generated from our huffman tree earlier. 
Lastly, it will flush and close all file streams. 
*/
void HuffmanTree::privHuff()
{
	makeSymbolTable();
	//symTable.dump(freqCount);
	std::string outputFile = file.substr(0, file.find(".")); //Otherwise, it'll output it as 'common-sense.txt.huff'
	std::ofstream huffFile(outputFile + ".huff"); //output to encoded file
	std::ifstream inFile(file); //Input from original file
	BitWriter bitwriter(&huffFile);
	unsigned char zeroLength = 0;
	unsigned long long charCode;
	int length;
	for (int i = 0; i < 256; i++) {
		CodedSymbol sym = symTable.getSymbol(i);
		if (freqCount[i] == 0) {
			huffFile.put(zeroLength);
		}
		else {
			huffFile.put(sym.getBitLength());
		}

		if (freqCount[i] != 0) { //Push the bits of only the character values that have a length > 0
			length = static_cast<int>(sym.getBitLength());
			charCode = sym.getCharCode();
			for (int i = 63; i > (63 - length); i--) { //Since an unsigned long long has 64 bits, I left shifted everything 
				bitwriter.putBit((charCode >> i) & 1);
			}
			bitwriter.flush();
		}
	}
	unsigned char flipEnd;
	for (int i = 0; i < 4; i++) {
		flipEnd = static_cast<unsigned char>(totalChars);
		huffFile.put(flipEnd);
		totalChars = totalChars >> 8;
	}
	int totalWrite = 0;
	char input;
	while (inFile.get(input)) {
		if (input != EOF) {
			CodedSymbol sym = symTable.getSymbol(input);
			length = static_cast<int>(sym.getBitLength());
			charCode = sym.getCharCode();
			for (int i = 63; i > (63 - length); i--) {
				bitwriter.putBit((charCode >> i) & 1);
				totalWrite++;
			}
		}
		else {
			break;
		}
	}
	//Explicitly flush and close file streams here. 
	//Destructor would normally handle this operation. 
	bitwriter.flush();
	inFile.close();
	huffFile.close();
}

/*
Precondition: None
Postcondition: None
Decoding function. It will take in a file name (string) and replace the extension with .puff indicating the output file. 
It will read in our .huff file and begin constructing the binary codes once more based on the bytes passed through. 
The function will utilize a bitreader class (see BitReader.h and BitReaper.cpp for implementation details). 
Utilizing this class, it will read in the bits represented as unsigned ints and construct the binary code for every character.
If the character doesn't appear, the function will skip over the byte and keep reading. 
The function then byte manipulates the last 4 bytes in our coded symbols header to get back our
unsigned integer representing the total number of characters. 
From there, it will read the rest of the text file bit-by-bit, writing out to the file once we've hit a leaf node 
until the file has been completely read. 
*/
void HuffmanTree::privPuff(std::string fileName)
{
	std::ifstream inFile(fileName);
	std::string puffName = fileName.substr(0, fileName.find(".") + 1) + "puff";
	std::ofstream puffFile(puffName);
	//std::ofstream puffFile("OUTPUTTEST.txt");
	BitReader bitReader(&inFile);
	int bitLength = 0;
	unsigned int bit;
	huffmanRoot = new Node<unsigned int, unsigned char>();
	Node<unsigned int, unsigned char>* temp = huffmanRoot;
	//BUILDING THE HUFFMAN TREE
	for (int i = 0; i < 256; i++) {
		temp = huffmanRoot;
		bitLength = static_cast<int>(bitReader.getByte()); //bitlength
		if (bitLength > 0) {
			for (int j = 0; j < bitLength - 1; j++) {
				bit = bitReader.getBit();
				if (bit == 0) { //Build left
					if (!temp->getLeft()) {
						temp->setLeft(new Node<unsigned int, unsigned char>());
					}
					temp = temp->getLeft();
				}
				else if (bit == 1) { //Build right
					if (!temp->getRight()) {
						temp->setRight(new Node<unsigned int, unsigned char>());
					}
					temp = temp->getRight();
				}
			}
			bit = bitReader.getBit();
			if (bit == 0) {
				temp->setLeft(new Node<unsigned int, unsigned char>(true, (char)i));
			}
			else if (bit == 1) {
				temp->setRight(new Node<unsigned int, unsigned char>(true, (char)i));
			}
		}
	}
	//The int is broken up into 4 separate unsigned char bytes. 
	unsigned char bytes[4];
	for (int i = 0; i < 4; i++) {
		bytes[i] = bitReader.getByte();
	}
	//Bytes are in reverse order, need to shift to reverse and OR. 
	totalChars |= (bytes[0]);
	totalChars |= (bytes[1] << 8);
	totalChars |= (bytes[2] << 16);
	totalChars |= (bytes[3] << 24);
	//test(huffmanRoot, "");

	//DECODING THE TEXT.
	unsigned int numDecoded = 0;
	temp = huffmanRoot;
	while (numDecoded != totalChars) { //Stops when we've decoded the number of characters total. 
		if (!temp->getLeft() && !temp->getRight()) {
			puffFile.put((char)temp->getData());
			//std::cout << temp->getData();
			numDecoded++;
			temp = huffmanRoot;
		}
		else {
			bit = bitReader.getBit();
			if (bit == 0) {
				temp = temp->getLeft();
			}
			else if (bit == 1) {
				temp = temp->getRight();
			}
		}
	}
	puffFile.close();
}

//Public interface method for decoding
void HuffmanTree::puff(std::string fileName)
{
	privPuff(fileName);
}

//Prints out the binary representation in a string form. 
void HuffmanTree::test(Node<unsigned int, unsigned char>* node, string code)
{
	if (node) {
		if (!node->getLeft() && !node->getRight()) {
				std::cerr << node->getData() << " " << code << std::endl;
		}
		test(node->getLeft(), code + "0");
		test(node->getRight(), code + "1");
	}

}

//Dumps the tree in a "tree-like" pattern
void HuffmanTree::dumpTree()
{
	privDumpTree(1, huffmanRoot); 
}

//Private helper method for dumping
void HuffmanTree::privDumpTree(int length, Node<unsigned int, unsigned char>* node)
{
	string branch = "--+";
	if (node) {
		privDumpTree(length + 2, node->getLeft());
		for (int i = 0; i < length; i++) {
			cerr << branch;
		}
		cerr << "DATA: " << node->getData() << ", Count: " << node->getPriority() << std::endl; 
		privDumpTree(length + 2, node->getRight());
	}
}
