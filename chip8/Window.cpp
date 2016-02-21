
#include "Window.h"

static bool inputKeys[SDLK_LAST];

Pixel::Pixel()
{
	position.x = -1;
	position.y = -1;
	color = BLACK_PIXEL;
}

Window::Window():
WINDOW_TITLE ("Chip8 Emulator v0.1")
{
	screen = NULL;
	quit = false;
	pixelScale = 10;
	width = SCREEN_W * pixelScale;
	height = SCREEN_H * pixelScale;
	initInputKeys();

	for(int y = 0; y < SCREEN_H; y++)
	{
		for(int x = 0; x < SCREEN_W; x++)
		{
			pixels[y][x].position.x = x * pixelScale;
			pixels[y][x].position.y = y * pixelScale;
		}
	}

	for(int i = 0; i < NB_COLORS; i++)
		pixelSurfaces[i] = NULL;

	initSDL();
}

Window::~Window()
{
	for(int i = 0; i < NB_COLORS; i++)
		SDL_FreeSurface(pixelSurfaces[i]);

	SDL_Quit();
}

void Window::initSDL()
{
	//Init SDL
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(width, height, 32, SDL_DOUBLEBUF | SDL_HWSURFACE);
	if(!screen) { printf("SDL init failed\n"); exit(-1); }

	SDL_WM_SetCaption(WINDOW_TITLE, NULL);

	//Init color surfaces
	for(int i = 0; i < NB_COLORS; i++)
	{
		pixelSurfaces[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, pixelScale, pixelScale, 32, 0, 0, 0, 0);
		if(!pixelSurfaces[i]) { printf("SDL init failed\n"); exit(-1); }
	}

	SDL_FillRect(pixelSurfaces[BLACK_PIXEL], NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_FillRect(pixelSurfaces[WHITE_PIXEL], NULL, SDL_MapRGB(screen->format, 255, 255, 255));
}

void Window::render()
{
	draw();
	SDL_Flip(screen);
}

void Window::draw()
{
	for(int y = 0; y < SCREEN_H; y++)
		for(int x = 0; x < SCREEN_W; x++)
			SDL_BlitSurface(pixelSurfaces[pixels[y][x].color], NULL, screen, &(pixels[y][x].position));
}

void Window::update(Cpu& cpu)
{
	for(int y = 0; y < SCREEN_H; y++)
		for(int x = 0; x < SCREEN_W; x++)
			pixels[y][x].color = cpu.getScreen().getPixel(x, y);
}

void Window::initInputKeys()
{
	for(int i = 0; i < SDLK_LAST; i++)
		inputKeys[i] = SDL_RELEASED;
}

void Window::updateInput()
{
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_KEYDOWN:
			inputKeys[event.key.keysym.sym] = SDL_PRESSED;
			break;
		case SDL_KEYUP:
			inputKeys[event.key.keysym.sym] = SDL_RELEASED;
			break;
		case SDL_QUIT:
			quit = true;
			break;
		default:
			break;
		}
	}
}

void Window::handleKeyboard(Cpu& cpu)
{
	cpu.setKey(0x1, inputKeys[SDLK_1]);
	cpu.setKey(0x2, inputKeys[SDLK_2]);
	cpu.setKey(0x3, inputKeys[SDLK_3]);
	cpu.setKey(0xC, inputKeys[SDLK_4]);
	cpu.setKey(0x4, inputKeys[SDLK_q]);
	cpu.setKey(0x5, inputKeys[SDLK_w]);
	cpu.setKey(0x6, inputKeys[SDLK_e]);
	cpu.setKey(0xD, inputKeys[SDLK_r]);
	cpu.setKey(0x7, inputKeys[SDLK_a]);
	cpu.setKey(0x8, inputKeys[SDLK_s]);
	cpu.setKey(0x9, inputKeys[SDLK_d]);
	cpu.setKey(0xE, inputKeys[SDLK_f]);
	cpu.setKey(0xA, inputKeys[SDLK_z]);
	cpu.setKey(0x0, inputKeys[SDLK_x]);
	cpu.setKey(0xB, inputKeys[SDLK_c]);
	cpu.setKey(0xF, inputKeys[SDLK_v]);
}

void Window::wait() const
{
	const unsigned int WAIT_TIME = 1000.0f / FPS;
	static unsigned int precedentTime = 0;

	unsigned int elapsedTime = SDL_GetTicks() - precedentTime;
	if(elapsedTime < WAIT_TIME)
		SDL_Delay(WAIT_TIME - elapsedTime);
	precedentTime = SDL_GetTicks();
}