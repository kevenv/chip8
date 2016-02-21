
#include "BinaryFile.h"

#include <cstdio>
#include <cstdlib>
#include "Debugger.h"

BinaryFile::BinaryFile(char* fileName)
{
	file = NULL;
	size = -1;
	load(fileName);
}

BinaryFile::~BinaryFile()
{
	delete file;
}

void BinaryFile::load(char* fileName)
{
	FILE* file = fopen(fileName, "rb");
	//Open file
	if(!file)
	{
		printf("File '%s' can't be opened", fileName);
		exit(-1);
	}

	//Get file size
	fseek(file, 0, SEEK_END);
	const long fileSize = ftell(file);
	rewind(file);

	//Allocate buffer
	unsigned char* buffer = new unsigned char[fileSize];
	if(buffer == NULL)
	{
		printf("Can't allocate '%d' bytes", fileSize);
		exit(-1);
	}

	//Put file in buffer
	if(fread(buffer, 1, fileSize, file) != fileSize)
	{
		printf("Can't put file '%s' in buffer", fileName);
		exit(-1);
	}

	this->file = buffer;
	size = fileSize;

	fclose(file);
}

void BinaryFile::print() const
{
	int nbBytes = 0;
	while(nbBytes < size)
	{
		//Print binary representation of the file
		const int c  = file[nbBytes];
		printBinNumber(c);

		nbBytes++;

		//Format
		if(nbBytes % 8 == 0)
			printf("\n");
		else
			printf(" ");
	}

	printf("\nTotal bytes read: %d", nbBytes);
}
