#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


#define MEM_SIZE 4096
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

/*----- Macros -----*/
#define vF v[0xF]
#define cpu_fetch(pc) (memory[pc] << 8 | memory[pc+1])

#define cpu_getXReg(op) (((op) & 0x0F00) >> 8)
#define cpu_getYReg(op) (((op) & 0x00F0) >> 4)

#define cpu_getN(op) ((op) & 0x000F)
#define cpu_getNN(op) ((op) & 0x00FF)
#define cpu_getNNN(op) ((op) & 0x0FFF)

/*----- Shared Variable -----*/
unsigned char gfx[64 * 32];		// Array for graphics 64 x 32
unsigned char drawFlag;		// Flag for drawing
unsigned char keypad[16];	// Keypad
unsigned char v[16];			// CPU registers
unsigned short I;				// Index register 0x000 to 0xFFF
unsigned char memory[MEM_SIZE]; // Memory: 4k in total

void cpu_initialize();
void emulateCycle();
bool cpu_loadRom(char* file);



