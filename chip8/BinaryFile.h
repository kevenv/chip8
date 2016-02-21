#ifndef DEF_BINARY_FILE_H
#define DEF_BINARY_FILE_H

class BinaryFile
{
public:
	BinaryFile(char* fileName);
	~BinaryFile();

	void load(char* fileName);
	void print() const;

	inline long getSize() const { return size; }
	inline unsigned char* getFile() const { return file; }

private:
	unsigned char* file;
	long size; //In bytes
};

#endif