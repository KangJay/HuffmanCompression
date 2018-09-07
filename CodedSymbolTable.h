#pragma once
///Author: Ji Kang
///Homework Assignment 2: Huffman Compression
///CSS 343, Prof. Bernstein
///CodedSymbol Table class as well as the CodedSymbol class. 
///Class aids in store the binary representation of given byte inputs. It can set, get, and store
///these values for use further on. Has individual bit manipulation methods to help store. 
///Class assumes the binary representations can be stored within an unsigned long long (64 bits). 

/*
Attempted to have this class as a dependent internal class of CodedSymbolTable but ran into complications
when I was first filling in the coded symbols table for the recursive call. Made it a separate class on its
own to be able to create the table. See "makeSymbolTable()" method in Huffman.h for further explanation. 
*/
class CodedSymbol {
public:
	CodedSymbol() : bitLength(0), charCode(0), isUsed(false) {}
	CodedSymbol(unsigned char length, unsigned long long code) : bitLength(length), charCode(code), isUsed(true) {}
	unsigned char getBitLength() { return bitLength; }
	unsigned long long getCharCode() { return charCode; }
	bool getUsed() { return isUsed; }
	void pushBit(unsigned int bit);
	void fillBit();
private:
	unsigned char bitLength;
	unsigned long long charCode;
	bool isUsed;
};

class CodedSymbolTable {
private:

	CodedSymbol codeTable[256];

public:
	CodedSymbolTable();
	void setCharCode(unsigned char index, CodedSymbol sym);
	CodedSymbol getSymbol(unsigned char index) { return codeTable[index]; }
	void dump(unsigned int freqCount[256]);
};
