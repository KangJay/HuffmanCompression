#include "CodedSymbolTable.h"
#include <iostream>

using namespace std;

/*
Precondition: None
Postcondition: None
Method takes in an unsigned int representing a bit and OR-EQUALS it
with the byte to store it. 
*/
void CodedSymbol::pushBit(unsigned int bit)
{
	bitLength++;
	if (bit == 1) {
		charCode |= 1;
	}
	charCode <<= 1;
}

/*
Precondition: None
Postcondition: None
Helper method to fill in remaining bits by left shifting. 
*/
void CodedSymbol::fillBit()
{
	for (int i = bitLength; i < 63; i++) {
		charCode = charCode << 1;
	}
}

/*
Precondition: None
Postcondition: None
Constructor to intiially set all coded symbols to default.
*/
CodedSymbolTable::CodedSymbolTable()
{
	for (int i = 0; i < 255; i++) {
		codeTable[i] = CodedSymbol(); //All symbols will be 0 length 0 (empty) code by default. 
	}
}

/*
Precondition: None
Postcondition: None
Constructor that'll create a CodedSymbol object holding specific data. 
*/
void CodedSymbolTable::setCharCode(unsigned char index, CodedSymbol sym)
{
	codeTable[index] = CodedSymbol(sym.getBitLength(), sym.getCharCode());
}

/*
Precondition: None
Postcondition: None
Dump verification function that'll help visualize the binary representation of each
character as well as its frequency. 
*/
void CodedSymbolTable::dump(unsigned int freqCount[256])
{
	int totalBits = 0; 
	int ind = 0;
	for (int i = 0; i < 256; i++) {
		unsigned long long charCode = codeTable[i].getCharCode();
		int length = static_cast<int>(codeTable[i].getBitLength());
		if (length != 0) {
			ind = 0;
			totalBits += (length * freqCount[i]);
			cout << "CHARACTER: " << (char)i << ", BIT LENGTH: " << length << ", FREQUENCY: " << freqCount[i] << endl;
			for (int i = 63; i >= 0; i--) {
				if (ind == length) {
					break;
				}
				std::cerr << ((charCode >> i) & 1);
				ind++;
			}
			std::cerr << std::endl << std::endl;
		}
	}
	std::cerr << "Total Bits used: " << totalBits << std::endl;
}

