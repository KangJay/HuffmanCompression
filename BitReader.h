#pragma once
#include <fstream>
///Author: Ji Kang
///Homework Assignment 2: Huffman Compression
///CSS 343, Prof. Bernstein
///Bit input reader stream. Class is meant to read in bits from a single byte. This byte is represented as an unsigned char variable and will be reset 
///each time we hit the limit count of 8 since there are 8 bits in a byte. It will either read in single bits or read a whole byte in depending on the
///explicit method calls to objects of this class. 
///Class utilizes ifstream to get char inputs from a file (Binary in the case of huffman compression). 

class BitReader {
public:
	BitReader(std::ifstream* file) : byte('0'), puffFile(file), count(0) {}
	~BitReader() {}
	unsigned int getBit();
	unsigned char getByte();
private:
	unsigned char byte; //Byte field
	std::ifstream* puffFile; //Input file
	int count; //Bit count
};
