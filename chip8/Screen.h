#ifndef DEF_SCREEN_H
#define DEF_SCREEN_H

#define SCREEN_W 64
#define SCREEN_H 32

#define BLACK_PIXEL 0x0
#define WHITE_PIXEL 0x1

class Screen
{
public:
	Screen();
	~Screen() {};

	void clear();
	void print() const;
	void fill();
	void setPixel(unsigned char x, unsigned char y);
	unsigned char getPixel(unsigned char x, unsigned char y) const;

private:
	//Screen of 64x32 pixels monochrome
	//0x0 = black pixel
	//0x1 = white pixel
	unsigned char screen[SCREEN_H][SCREEN_W];
};

#endif