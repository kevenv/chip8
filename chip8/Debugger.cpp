
#include "Debugger.h"

#include <iostream>

void printBinNumber(unsigned char number)
{
	int output[8];
	for(int i = 0; i < 8; i++)
	{
		output[i] = (number >> i) & 0x1; //Extrach each bit
		if( i!= 0 && (i % 4 == 0) ) //Format group 4 bits
			std::cout << " ";
		std::cout << output[i];
	}
	std::cout << std::endl;
}
