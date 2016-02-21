
#include "Screen.h"
#include <iostream>

Screen::Screen()
{
	clear();
}

void Screen::clear()
{
	for(int y = 0; y < SCREEN_H; y++)
		for(int x = 0; x < SCREEN_W; x++)
			screen[y][x] = BLACK_PIXEL;
}

void Screen::print() const
{
	for(int y = 0; y < SCREEN_H; y++)
	{
		for(int x = 0; x < SCREEN_W; x++)
		{
			const unsigned char c = screen[y][x] == BLACK_PIXEL?'_':'*';
			std::cout << c;
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}

void Screen::fill()
{
	for(int y = 0; y < SCREEN_H; y++)
		for(int x = 0; x < SCREEN_W; x++)
			screen[y][x] = WHITE_PIXEL;
}

void Screen::setPixel(unsigned char x, unsigned char y)
{
	if(x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H)
		screen[y][x] ^= 0x1; //XOR draw
	else
		std::cout << "error: trying to set pixel at " << (int)x << "," << (int)y << std::endl;
}

unsigned char Screen::getPixel(unsigned char x, unsigned char y) const
{
	if(x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H)
		return screen[y][x];
	else
		std::cout << "error: trying to get pixel at " << (int)x << "," << (int)y << std::endl;
}
