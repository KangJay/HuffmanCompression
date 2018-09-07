#include "BitReader.h"
///See header file for documentation details. 

/*
Precondition: None
Postcondition: Returns an unsigned int.
Method returns a single unsigned int representing the binary bit. 
*/
unsigned int BitReader::getBit()
{
	unsigned int bit; 
	if (count == 0) {
		byte = puffFile->get(); 
		count = 8;
	}
	count--; 
	//Right shifts the byte by the count amount and "ANDS" it with 1 to set 1 if the bit within the byte is 1 and 0 otherwise. 
	bit = ((byte >> count) & 1);
	return bit;
}

/*
Precondition: None
Postcondition: Returns a unsigned char
Method utilized to skip to the next byte location and return that. 
*/
unsigned char BitReader::getByte()
{
	count = 0; 
	byte = puffFile->get();
	return byte;
}
