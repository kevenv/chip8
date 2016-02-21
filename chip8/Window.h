#ifndef DEF_WINDOW_H
#define DEF_WINDOW_H

#include <SDL/SDL.h>
#include "Screen.h"
#include "Cpu.h"

#define NB_COLORS 2

#define FPS 60

class Pixel
{
	friend class Window;
public:
	Pixel();

private:
	SDL_Rect position;
	Uint32 color;
};

class Window
{
public:
	Window();
	~Window();

	void wait() const;
	void updateInput();
	void update(Cpu& cpu);
	void render();
	void draw();

	void Window::handleKeyboard(Cpu& cpu);

	inline bool isQuit() const { return quit; }
	//inline void setPixel(int x, int y, int color) { pixels[y][x].color = color; }

private:
	SDL_Surface* screen;
	SDL_Event event;
	bool quit;

	int pixelScale;
	int width;
	int height;
	const char* WINDOW_TITLE;

	Pixel pixels[SCREEN_H][SCREEN_W];
	SDL_Surface* pixelSurfaces[NB_COLORS];

	void initSDL();
	void initInputKeys();
};

#endif