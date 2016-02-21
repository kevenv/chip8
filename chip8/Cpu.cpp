
#include "Cpu.h"

#include <iostream>
#include <ctime>
#include "BinaryFile.h"

const unsigned char Cpu::fontSet[NB_FONTS * FONT_HEIGHT] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const unsigned short Cpu::opcodeMask[NB_INSTRUCTIONS] =
{
	0x0000, /* 0NNN */
	0xFFFF, /* 00E0 */
	0xFFFF, /* 00EE */
	0xF000, /* 1NNN */
	0xF000, /* 2NNN */
	0xF000, /* 3XNN */
	0xF000, /* 4XNN */
	0xF00F, /* 5XY0 */
	0xF000, /* 6XNN */
	0xF000, /* 7XNN */
	0xF00F, /* 8XY0 */
	0xF00F, /* 8XY1 */
	0xF00F, /* 8XY2 */
	0xF00F, /* BXY3 */
	0xF00F, /* 8XY4 */
	0xF00F, /* 8XY5 */
	0xF00F, /* 8XY6 */
	0xF00F, /* 8XY7 */
	0xF00F, /* 8XYE */
	0xF00F, /* 9XY0 */
	0xF000, /* ANNN */
	0xF000, /* BNNN */
	0xF000, /* CXNN */
	0xF000, /* DXYN */
	0xF0FF, /* EX9E */
	0xF0FF, /* EXA1 */
	0xF0FF, /* FX07 */
	0xF0FF, /* FX0A */
	0xF0FF, /* FX15 */
	0xF0FF, /* FX18 */
	0xF0FF, /* FX1E */
	0xF0FF, /* FX29 */
	0xF0FF, /* FX33 */
	0xF0FF, /* FX55 */
	0xF0FF /* FX65 */
};

const unsigned short Cpu::opcodeID[NB_INSTRUCTIONS] = 
{
	0x0FFF, /* 0NNN */
	0x00E0, /* 00E0 */
	0x00EE, /* 00EE */
	0x1000, /* 1NNN */
	0x2000, /* 2NNN */
	0x3000, /* 3XNN */
	0x4000, /* 4XNN */
	0x5000, /* 5XY0 */
	0x6000, /* 6XNN */
	0x7000, /* 7XNN */
	0x8000, /* 8XY0 */
	0x8001, /* 8XY1 */
	0x8002, /* 8XY2 */
	0x8003, /* BXY3 */
	0x8004, /* 8XY4 */
	0x8005, /* 8XY5 */
	0x8006, /* 8XY6 */
	0x8007, /* 8XY7 */
	0x800E, /* 8XYE */
	0x9000, /* 9XY0 */
	0xA000, /* ANNN */
	0xB000, /* BNNN */
	0xC000, /* CXNN */
	0xD000, /* DXYN */
	0xE09E, /* EX9E */
	0xE0A1, /* EXA1 */
	0xF007, /* FX07 */
	0xF00A, /* FX0A */
	0xF015, /* FX15 */
	0xF018, /* FX18 */
	0xF01E, /* FX1E */
	0xF029, /* FX29 */
	0xF033, /* FX33 */
	0xF055, /* FX55 */
	0xF065, /* FX65 */
};

Cpu::Cpu()
{
	opcode = 0;
	pc = 0x200; //Program memory start at 0x200
	I = 0;
	sp = 0;

	clearRegisters();
	clearStack();
	clearRAM();
	
	DT = 0;
	ST = 0;

	loadFontSet();

	jump = false;
	drawFlag = false;

	clearKeyboard();

	//init random seed
	srand(time(NULL));
}

Cpu::~Cpu()
{

}

void Cpu::tick()
{
	for(int i = 0; i < CYCLES_BY_SECOND; i++)
	{
		fetchOpcode();
		const OPCODE opcode = decodeOpcode();
		executeOpcode(opcode);
		if(!jump)
			pc+=2;
		else
			jump = false; //TODO: possible problem of sync because timers are updated before the actual jump
	}

	updateTimers();
}

void Cpu::fetchOpcode()
{
	/*
	The instruction is coded on 2 bytes, 
	so we assemble the 2 parts together to get the full instruction
	part1 = 1111 1111
	part2 = 1010 1010
	opcode = 0000 0000 0000 0000
	 << 8  = 1111 1111 ____ ____
		OR
			 = ____ ____ 1010 1010
			 = 1111 1111 1010 1010
	*/

	opcode = (ram[pc] << 8) | ram[pc+1];
}

Cpu::OPCODE Cpu::decodeOpcode() const
{
	unsigned int opcode;
	for(opcode = 0; opcode < NB_INSTRUCTIONS; opcode++)
	{
		const unsigned short result = this->opcode & Cpu::opcodeMask[opcode];
		if(result == Cpu::opcodeID[opcode])
			break;
	}

	return (OPCODE) opcode;
}

void Cpu::executeOpcode(int instructionID)
{
	//TODO: X Y N NN NNN -> ONE TIME
	switch(instructionID)
	{
	case _0NNN: //ignored
		break;
	case _00E0: //clear screen
		screen.clear();
		break;
	case _00EE: //return back to subroutine
		sp--;
		pc = stack[sp];
		break;
	case _1NNN: //jump to NNN
		pc = opcode & ~Cpu::opcodeMask[_1NNN];
		jump = true;
		break;
	case _2NNN: //call subroutine at NNN
		//keep last address on the stack
		stack[sp] = pc;
		sp++;
		//call subroutine
		pc = opcode & ~Cpu::opcodeMask[_2NNN];
		jump = true;
		break;
	case _3XNN: //skip next instruction if VX == NN
		if( V[ (opcode & 0x0F00) >> 8 ] == (opcode & 0x00FF) )
			pc += 2;
		break;
	case _4XNN: //Skip next instruction if VX != NN
		if( V[ (opcode & 0x0F00) >> 8 ] != (opcode & 0x00FF) )
			pc += 2;
		break;
	case _5XY0: //skip next instruction if VX == VY
		if( V[ (opcode & 0x0F00) >> 8 ] != (opcode & 0x00F0) >> 4)
			pc += 2;
		break;
	case _6XNN: //VX = NN
		V[ (opcode & 0x0F00) >> 8 ] = opcode & 0x00FF;
		break;
	case _7XNN: //VX = VX + NN
		V[ (opcode & 0x0F00) >> 8 ] += opcode & 0x00FF;
		break;
	case _8XY0: //VX = VY
		V[ (opcode & 0x0F00) >> 8 ] = V[ (opcode & 0x00F0) >> 4 ];
		break;
	case _8XY1: //VX = VX OR VY
		V[ (opcode & 0x0F00) >> 8 ] |= V[ (opcode & 0x00F0) >> 4 ];
		break;
	case _8XY2: //VX = VX AND VY
		V[ (opcode & 0x0F00) >> 8 ] &= V[ (opcode & 0x00F0) >> 4 ];
		break;
	case _8XY3: //VX = VX XOR VY
		V[ (opcode & 0x0F00) >> 8 ] ^= V[ (opcode & 0x00F0) >> 4 ];
		break;
	case _8XY4: //VX = VX + VY (VF = 1 if carry)
		V[0xF] = (V[ (opcode & 0x0F00) >> 8 ] + V[ (opcode & 0x00F0) >> 4 ]) > 0xFF ? 1 : 0; //carry
		V[ (opcode & 0x0F00) >> 8 ] += V[ (opcode & 0x00F0) >> 4 ]; //addition
		break;
	case _8XY5: //VX = VX - VY (VF = 0 if borrow)
		V[0xF] = V[ (opcode & 0x0F00) >> 8 ] < V[ (opcode & 0x00F0) >> 4 ] ? 0 : 1 ; //borrow
		V[ (opcode & 0x0F00) >> 8 ] -= V[ (opcode & 0x00F0) >> 4 ]; //substraction
		break;
	case _8XY6: //VF = least significant bit of VX, VX = VX >> 1
		V[0xF] = V[ (opcode & 0x0F00) >> 8 ] & 0x1;
		V[ (opcode & 0x0F00) >> 8 ] >>= 0x1;
		break;
	case _8XY7: //VX = VY - VX (VF = 0 if borrow)
		V[0xF] = V[ (opcode & 0x00F0) >> 4 ] < V[ (opcode & 0x0F00) >> 8 ] ? 0 : 1 ; //borrow
		V[ (opcode & 0x0F00) >> 8 ] -= V[ (opcode & 0x00F0) >> 4 ]; //substraction
		break;
	case _8XYE: //VF = most significant bit of VX, VX = VX << 1
		V[0xF] = V[ (opcode & 0x0F00) >> 8 ] & 0x80;
		V[ (opcode & 0x0F00) >> 8 ] <<= 0x1;
		break;
	case _9XY0: //skip next instruction if VX != VY
		if( V[ (opcode & 0x0F00) >> 8 ] != V[ (opcode & 0x00F0) >> 4] )
			pc += 2;
		break;
	case _ANNN: //I = NNN
		I = opcode & ~Cpu::opcodeMask[_ANNN];
		break;
	case _BNNN: //jump to NNN + V0
		pc = (opcode & ~opcodeMask[_BNNN]) + V[0x0];
		jump = true;
		break;
	case _CXNN: //VX = random byte (0-FF) AND NN
		V[ (opcode & 0x0F00) >> 8 ] = (rand() % 0xFF + 1) & (opcode & 0x00FF);
		break;
	case _DXYN: //Draw a 8xN sprite at (VX, VY)
		instrDXYN();
		break;
	case _EX9E: //skip next instruction if key stored in VX is pressed
		if( keyboard[ V[(opcode & ~opcodeMask[_EX9E]) >> 8] ] == true)
			pc += 2;
		break;
	case _EXA1: //Skip next instruction if key stored in VX isn't pressed
		if( keyboard[ V[(opcode & ~opcodeMask[_EXA1]) >> 8] ] == false)
			pc += 2;
		else
		{
			std::cout << "chip8 : press detected" << std::endl;
			for(unsigned char i = 0; i < NB_KEYS; i++)
				std::cout << (int)i << ": " << keyboard[i] << std::endl;
		}
		break;
	case _FX07: //VX = DT
		V[ (opcode & ~opcodeMask[_FX07]) >> 8 ] = DT;
		break;
	case _FX0A: //wait for key press, VX = key pressed
		std::cout << "Instruction 'FX0A' is not implemented" << std::endl;
		break;
	case _FX15: //DT = VX
		DT = V[ (opcode & ~opcodeMask[_FX15]) >> 8 ];
		break;
	case _FX18: //ST = VX
		ST = V[ (opcode & ~opcodeMask[_FX18]) >> 8 ];
		break;
	case _FX1E: //I = I + VX
		I += V[ (opcode & ~opcodeMask[_FX1E]) >> 8 ];
		break;
	case _FX29: //I = address of the sprite for the character in VX
		I = V[ (opcode & ~opcodeMask[_FX29]) >> 8 ] * FONT_HEIGHT;
		break;
	case _FX33: //Store binary-coded decimal representation of VX at I, I+1, I+2
		instrFX33();
		break;
	case _FX55: //store V0 to VX at address I + ...
		for(unsigned char i = 0; i <= (opcode & ~opcodeMask[_FX55]) >> 8; i++)
			ram[I + i] = V[i];
		break;
	case _FX65: //fill V0 to VX with values from I + ...
		for(unsigned char i = 0; i <= (opcode & ~opcodeMask[_FX65]) >> 8; i++)
			V[i] = ram[I+i];
		break;
	default:
		std::cout << "Instruction '" << instructionID << "' not implemented" << std::endl;
		int i;
		std::cin >> i;
		break;
	}
}

void Cpu::clearRegisters()
{
	for(int i = 0; i < NB_REGISTERS; i++)
		V[i] = 0;
}

void Cpu::clearRAM()
{
	for(int i = 0; i < BYTES_OF_RAM; i++)
		ram[i] = 0;
}

void Cpu::clearStack()
{
	for(int i = 0; i < STACK_SIZE; i++)
		stack[i] = 0;
}

void Cpu::clearKeyboard()
{
	for(unsigned char i = 0; i < NB_KEYS; i++)
		keyboard[i] = false;
}

void Cpu::loadFontSet()
{
	//Font are loaded from beginning of ram
	for(int i = 0; i < NB_FONTS*FONT_HEIGHT; i++)
		ram[i] = Cpu::fontSet[i];
}

void Cpu::loadROM(char* fileName)
{
	const BinaryFile file(fileName);
	
	//Load ROM in RAM of the Chip8
	//Programs memory start at 0x200
	for(int i = 0; i < file.getSize(); i++)
		ram[i + 0x200] = file.getFile()[i];
}

void Cpu::updateTimers()
{
	//Update delay timer
	if(DT > 0)
		DT--;

	//Update sound timer
	if(ST > 0)
		ST--;
}

void Cpu::instrDXYN()
{
	//TODO: Possible problem
	//implement : If the sprite is positioned so part of it is outside the coordinates of the display, 
	//it wraps around to the opposite side of the screen.
	const unsigned int x = V[(opcode & 0x0F00) >> 8]; //get X screen coor from VX register
	const unsigned int y = V[(opcode & 0x00F0) >> 4]; //get Y screen coor from VX register
	const unsigned char n = opcode & 0x000F; //get height of the sprite from opcode

	V[0xF] = 0;
	for(int i = 0; i < n; i++)
	{
		const unsigned char line = ram[I + i]; //get first line of the sprite from ram
		for(int j = 0; j < 8; j++)
		{
			unsigned char pixel = line & (0x80 >> j); //extract 1 pixel of the sprite (bit)
			if(pixel != 0)
			{
				if(screen.getPixel(x+j, y+i) == WHITE_PIXEL) //if the pixel on the screen will be erased
					V[0xF] = 0x1; //VF is used as a drawFlag for collision detections

				screen.setPixel(x+j, y+i);
			}
		}
	}

	drawFlag = true;
}

void Cpu::instrFX33()
{
	unsigned char number = V[ (opcode & ~opcodeMask[_FX33]) >> 8 ];
	ram[I] = number / 100;
	ram[I+1] = (number % 100) / 10;
	ram[I+2] = (number % 100) % 10;
}