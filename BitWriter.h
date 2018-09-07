///Author: Ji Kang
///Homework Assignment 2: Huffman Compression
///CSS 343, Prof. Bernstein
///Bit writer stream program. Utilized in the encoding portion of the assignment. 
///Class is meant to be utilized reading in single bits from a given byte or a whole byte itself. 
///If a given byte is run out, the program will automatically pull in a byte from the file input stream. 
///
#pragma once
#include <fstream>

class BitWriter{
public:
	BitWriter() : byte('0'), count(0) {}
	BitWriter(std::ofstream* outFile) : byte('0'), huffFile(outFile), count (0) {}
	int putBit(unsigned int bit);
	int putByte(unsigned char byte);
	void flush();
private:
	unsigned char byte;
	std::ofstream* huffFile;
	int count;
};
