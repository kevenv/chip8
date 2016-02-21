#ifndef DEF_CPU_H
#define DEF_CPU_H

#include "Screen.h"

#define BYTES_OF_RAM 4096
#define NB_REGISTERS 16
#define STACK_SIZE 16
#define NB_INSTRUCTIONS 35
#define CYCLES_BY_SECOND 8

#define NB_FONTS 16
#define FONT_HEIGHT 5

#define NB_KEYS 16

typedef enum OPCODE OPCODE;

class Cpu
{
public:	
	static enum OPCODE {
		_0NNN,
		_00E0,
		_00EE,
		_1NNN,
		_2NNN,
		_3XNN,
		_4XNN,
		_5XY0,
		_6XNN,
		_7XNN,
		_8XY0,
		_8XY1,
		_8XY2,
		_8XY3,
		_8XY4,
		_8XY5,
		_8XY6,
		_8XY7,
		_8XYE,
		_9XY0,
		_ANNN,
		_BNNN,
		_CXNN,
		_DXYN,
		_EX9E,
		_EXA1,
		_FX07,
		_FX0A,
		_FX15,
		_FX18,
		_FX1E,
		_FX29,
		_FX33,
		_FX55,
		_FX65
	};

	Cpu();
	~Cpu();

	void tick();

	void fetchOpcode();
	OPCODE decodeOpcode() const;
	void executeOpcode(int instructionID);

	void updateTimers();
	inline void setKey(unsigned char keyID, bool pressed) { keyboard[keyID] = pressed; }

	void instrDXYN();
	void instrFX33();

	void clearRegisters();
	void clearRAM();
	void clearStack();
	void clearKeyboard();

	void loadFontSet();
	void loadROM(char* fileName);
	inline bool getDrawFlag() const { return drawFlag; }
	inline Screen& getScreen() { return screen; }

private:
	//Current instruction opcode (2 bytes)
	//Endianess : Big-endian (most significant bit first)
	unsigned short opcode;

	//Program counter (16 bits)
	//Store currently executing address
	unsigned short pc;

	//Data Registers (8 bits)
	//V0-VF
	//VF: carry flag
	//VF = 1 : need draw
	unsigned char V[NB_REGISTERS];

	//Address register (16 bits)
	unsigned short I;

	//Stack pointer (8 bits)
	//Point to the top level of the stack
	unsigned char sp;

	//Stack (16 bits)
	//Store the address that the interpreter should return to when finished with a subroutine
	unsigned short stack[STACK_SIZE];

	//Memory (4kb)
	//0x000-0x1FF : reserved for the interpreter
	//0x200-0xFFF : reserved for programs
	unsigned char ram[BYTES_OF_RAM];

	//Delay timer register
	//Delay timer subtract 1 from DT at 60hz
	//if DT != 0 delay timer is active
	unsigned char DT;

	//Sound timer register
	//Sound timer substract 1 from ST at 60hz
	//if ST != 0 sound timer is active -> BEEP
	unsigned char ST;

	bool jump; //true if a jump has been made

	bool drawFlag; //true if screen need to be refreshed
	Screen screen;

	bool keyboard[NB_KEYS];

	//Characters representation
	static const unsigned char fontSet[NB_FONTS * FONT_HEIGHT];

	static const unsigned short opcodeMask[NB_INSTRUCTIONS];

	static const unsigned short opcodeID[NB_INSTRUCTIONS];
};



#endif