#include "BitWriter.h"
#include <iostream>
//See corresponding header file for class documentation details. 
using namespace std;

/*
Precondition: None
Postcondition: Returns an int (Error testing). 
Takes in a single unsigned int, 'bit', representing the binary bit within a byte.
Method will buffer the bits (collect) until it has 8 total then it will invoke putByte(unsigned char); 
The count will reset to 0, indicating the byte buffer is now empty and ready to take more in. 
*/
int BitWriter::putBit(unsigned int bit)
{
	byte <<= 1; //Left shift by 1 to ensure the last bit place is able to be set
	if (bit == 1) {
		byte |= 1;
	}
	count++;
	if (count == 8) {
		count = 0; 
		putByte(byte); 
	}
	return 0;
}

/*
Precondition: 'character' is a full byte or a flushed full byte. 
Postcondition: Returns an int(error testing). 
Will take in a single byte, the unsigned char character in the parameter, and write out to file. 
*/
int BitWriter::putByte(unsigned char character)
{
	huffFile->put(character);
	return 0;
}

/*
Precondition: None
Postcondition: None
Essentially a helper function to shift all the bits leftward to ensure the byte is "big endian" bit-wise within the byte. 
*/
void BitWriter::flush()
{
	//Method is called every time the program writes out a the binary code for a given character. 
	//If a character has a binary code that's (length % 8 == 0), this method will write out a byte that's all
	//0's --> Catches and stop that. 
	if (count > 0) { 
		byte <<= (8 - count);
		huffFile->put(byte);
		count = 0;
	}
}
