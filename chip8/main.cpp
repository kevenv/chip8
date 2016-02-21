
#include <iostream>
#include "Cpu.h"
#include "Window.h"

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		std::cout << "Error: no ROM" << std::endl;
		int i;
		std::cin >> i;
		return -1;
	}

	Window w;
	Cpu cpu;
	cpu.loadROM(argv[1]);
	while(!w.isQuit())
	{
		w.updateInput();
		w.handleKeyboard(cpu);
		cpu.tick();

		if(cpu.getDrawFlag())
		{
			//cpu.getScreen().print();
			w.update(cpu);
			w.render();
		}

		w.wait();
	}

	return 0;
}
