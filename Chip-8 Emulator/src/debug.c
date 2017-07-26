#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "debug.h"

/*----- Debug Variables -----*/
unsigned short debugPC;
unsigned short debugOpcode;


unsigned char debugDelay_timer;
unsigned char debugSound_timer;
unsigned short debugStack[16];		// Stack and debugStack pointer
unsigned short debugSP;
bool debugDrawFlag;

unsigned char debugchip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

void debug_init()
{
	debugPC		= 0x200;		// Program counter starts at 0x200 (Start adress program)
	debugOpcode	= 0;			// Reset current debugOpcode
	debugI		= 0;			// Reset index register
	debugSP		= 0;			// Reset debugStack pointer

	// Clear display
	for(int i = 0; i < 2048; ++i)
		buff[i] = 0;

	// Clear debugStack
	for(int i = 0; i < 16; ++i)
		debugStack[i] = 0;

	for(int i = 0; i < 16; ++i)
		debugKey[i] = V[i] = 0;

	// Clear debugMemory
	for(int i = 0; i < 4096; ++i)
		debugMemory[i] = 0;

	// Load fontset
	for(int i = 0; i < 80; ++i)
		debugMemory[i] = debugchip8_fontset[i];

	// Reset timers
	debugDelay_timer = 0;
	debugSound_timer = 0;

	// Clear screen once
	debugDrawFlag = true;

	srand (time(NULL));
}

unsigned short debug_emulateCycle()
{
	// Fetch debugOpcode
	debugOpcode = debugMemory[debugPC] << 8 | debugMemory[debugPC + 1];

	// Process debugOpcode
	switch(debugOpcode & 0xF000)
	{
		case 0x0000:
			switch(debugOpcode & 0x000F)
			{
				case 0x0000: // 0x00E0: Clears the screen
					for(int i = 0; i < 2048; ++i)
						buff[i] = 0x0;
					debugDrawFlag = true;
					debugPC += 2;
				break;

				case 0x000E: // 0x00EE: Returns from subroutine
					--debugSP;			// 16 levels of debugStack, decrease debugStack pointer to prevent overwrite
					debugPC = debugStack[debugSP];	// Put the stored return address from the debugStack back into the program counter
					debugPC += 2;		// Don't forget to increase the program counter!
				break;

				default:
					printf ("Unknown debugOpcode [0x0000]: 0x%X\n", debugOpcode);
			}
		break;

		case 0x1000: // 0x1NNN: Jumps to address NNN
			debugPC = debugOpcode & 0x0FFF;
		break;

		case 0x2000: // 0x2NNN: Calls subroutine at NNN.
			debugStack[debugSP] = debugPC;			// Store current address in debugStack
			++debugSP;					// Increment debugStack pointer
			debugPC = debugOpcode & 0x0FFF;	// Set the program counter to the address at NNN
		break;

		case 0x3000: // 0x3XNN: Skips the next instruction if VX equals NN
			if(V[(debugOpcode & 0x0F00) >> 8] == (debugOpcode & 0x00FF))
				debugPC += 4;
			else
				debugPC += 2;
		break;

		case 0x4000: // 0x4XNN: Skips the next instruction if VX doesn't equal NN
			if(V[(debugOpcode & 0x0F00) >> 8] != (debugOpcode & 0x00FF))
				debugPC += 4;
			else
				debugPC += 2;
		break;

		case 0x5000: // 0x5XY0: Skips the next instruction if VX equals VY.
			if(V[(debugOpcode & 0x0F00) >> 8] == V[(debugOpcode & 0x00F0) >> 4])
				debugPC += 4;
			else
				debugPC += 2;
		break;

		case 0x6000: // 0x6XNN: Sets VX to NN.
			V[(debugOpcode & 0x0F00) >> 8] = debugOpcode & 0x00FF;
			debugPC += 2;
		break;

		case 0x7000: // 0x7XNN: Adds NN to VX.
			V[(debugOpcode & 0x0F00) >> 8] += debugOpcode & 0x00FF;
			debugPC += 2;
		break;

		case 0x8000:
			switch(debugOpcode & 0x000F)
			{
				case 0x0000: // 0x8XY0: Sets VX to the value of VY
					V[(debugOpcode & 0x0F00) >> 8] = V[(debugOpcode & 0x00F0) >> 4];
					debugPC += 2;
				break;

				case 0x0001: // 0x8XY1: Sets VX to "VX OR VY"
					V[(debugOpcode & 0x0F00) >> 8] |= V[(debugOpcode & 0x00F0) >> 4];
					debugPC += 2;
				break;

				case 0x0002: // 0x8XY2: Sets VX to "VX AND VY"
					V[(debugOpcode & 0x0F00) >> 8] &= V[(debugOpcode & 0x00F0) >> 4];
					debugPC += 2;
				break;

				case 0x0003: // 0x8XY3: Sets VX to "VX XOR VY"
					V[(debugOpcode & 0x0F00) >> 8] ^= V[(debugOpcode & 0x00F0) >> 4];
					debugPC += 2;
				break;

				case 0x0004: // 0x8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
					if(V[(debugOpcode & 0x00F0) >> 4] > (0xFF - V[(debugOpcode & 0x0F00) >> 8]))
						V[0xF] = 1; //carry
					else
						V[0xF] = 0;
					V[(debugOpcode & 0x0F00) >> 8] += V[(debugOpcode & 0x00F0) >> 4];
					debugPC += 2;
				break;

				case 0x0005: // 0x8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
					if(V[(debugOpcode & 0x00F0) >> 4] > V[(debugOpcode & 0x0F00) >> 8])
						V[0xF] = 0; // there is a borrow
					else
						V[0xF] = 1;
					V[(debugOpcode & 0x0F00) >> 8] -= V[(debugOpcode & 0x00F0) >> 4];
					debugPC += 2;
				break;

				case 0x0006: // 0x8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift
					V[0xF] = V[(debugOpcode & 0x0F00) >> 8] & 0x1;
					V[(debugOpcode & 0x0F00) >> 8] >>= 1;
					debugPC += 2;
				break;

				case 0x0007: // 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
					if(V[(debugOpcode & 0x0F00) >> 8] > V[(debugOpcode & 0x00F0) >> 4])	// VY-VX
						V[0xF] = 0; // there is a borrow
					else
						V[0xF] = 1;
					V[(debugOpcode & 0x0F00) >> 8] = V[(debugOpcode & 0x00F0) >> 4] - V[(debugOpcode & 0x0F00) >> 8];
					debugPC += 2;
				break;

				case 0x000E: // 0x8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
					V[0xF] = V[(debugOpcode & 0x0F00) >> 8] >> 7;
					V[(debugOpcode & 0x0F00) >> 8] <<= 1;
					debugPC += 2;
				break;

				default:
					printf ("Unknown debugOpcode [0x8000]: 0x%X\n", debugOpcode);
			}
		break;

		case 0x9000: // 0x9XY0: Skips the next instruction if VX doesn't equal VY
			if(V[(debugOpcode & 0x0F00) >> 8] != V[(debugOpcode & 0x00F0) >> 4])
				debugPC += 4;
			else
				debugPC += 2;
		break;

		case 0xA000: // ANNN: Sets debugI to the address NNN
			debugI = debugOpcode & 0x0FFF;
			debugPC += 2;
		break;

		case 0xB000: // BNNN: Jumps to the address NNN plus V0
			debugPC = (debugOpcode & 0x0FFF) + V[0];
		break;

		case 0xC000: // CXNN: Sets VX to a random number and NN
			V[(debugOpcode & 0x0F00) >> 8] = (rand() % 0xFF) & (debugOpcode & 0x00FF);
			debugPC += 2;
		break;

		case 0xD000: // DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
					 // Each row of 8 pixels is read as bit-coded starting from debugMemory location debugI;
					 // debugI value doesn't change after the execution of this instruction.
					 // VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
					 // and to 0 if that doesn't happen
		{
			unsigned short x = V[(debugOpcode & 0x0F00) >> 8];
			unsigned short y = V[(debugOpcode & 0x00F0) >> 4];
			unsigned short height = debugOpcode & 0x000F;
			unsigned short pixel;

			V[0xF] = 0;
			for (int yline = 0; yline < height; yline++)
			{
				pixel = debugMemory[debugI + yline];
				for(int xline = 0; xline < 8; xline++)
				{
					if((pixel & (0x80 >> xline)) != 0)
					{
						if(buff[(x + xline + ((y + yline) * 64))] == 1)
						{
							V[0xF] = 1;
						}
						buff[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}

			debugDrawFlag = true;
			debugPC += 2;
		}
		break;

		case 0xE000:
			switch(debugOpcode & 0x00FF)
			{
				case 0x009E: // EX9E: Skips the next instruction if the debugKey stored in VX is pressed
					if(debugKey[V[(debugOpcode & 0x0F00) >> 8]] != 0)
						debugPC += 4;
					else
						debugPC += 2;
				break;

				case 0x00A1: // EXA1: Skips the next instruction if the debugKey stored in VX isn't pressed
					if(debugKey[V[(debugOpcode & 0x0F00) >> 8]] == 0)
						debugPC += 4;
					else
						debugPC += 2;
				break;

				default:
					printf ("Unknown debugOpcode [0xE000]: 0x%X\n", debugOpcode);
			}
		break;

		case 0xF000:
			switch(debugOpcode & 0x00FF)
			{
				case 0x0007: // FX07: Sets VX to the value of the delay timer
					V[(debugOpcode & 0x0F00) >> 8] = debugDelay_timer;
					debugPC += 2;
				break;

				case 0x000A: // FX0A: A debugKey press is awaited, and then stored in VX
				{
					bool keyPress = false;

					for(int i = 0; i < 16; ++i)
					{
						if(debugKey[i] != 0)
						{
							V[(debugOpcode & 0x0F00) >> 8] = i;
							keyPress = true;
						}
					}

					// If we didn't received a keypress, skip this cycle and try again.
					if(!keyPress)
						return;

					debugPC += 2;
				}
				break;

				case 0x0015: // FX15: Sets the delay timer to VX
					debugDelay_timer = V[(debugOpcode & 0x0F00) >> 8];
					debugPC += 2;
				break;

				case 0x0018: // FX18: Sets the sound timer to VX
					debugSound_timer = V[(debugOpcode & 0x0F00) >> 8];
					debugPC += 2;
				break;

				case 0x001E: // FX1E: Adds VX to debugI
					if(debugI + V[(debugOpcode & 0x0F00) >> 8] > 0xFFF)	// VF is set to 1 when range overflow (debugI+VX>0xFFF), and 0 when there isn't.
						V[0xF] = 1;
					else
						V[0xF] = 0;
					debugI += V[(debugOpcode & 0x0F00) >> 8];
					debugPC += 2;
				break;

				case 0x0029: // FX29: Sets debugI to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
					debugI = V[(debugOpcode & 0x0F00) >> 8] * 0x5;
					debugPC += 2;
				break;

				case 0x0033: // FX33: Stores the Binary-coded decimal representation of VX at the addresses debugI, debugI plus 1, and debugI plus 2
					debugMemory[debugI]     = V[(debugOpcode & 0x0F00) >> 8] / 100;
					debugMemory[debugI + 1] = (V[(debugOpcode & 0x0F00) >> 8] / 10) % 10;
					debugMemory[debugI + 2] = (V[(debugOpcode & 0x0F00) >> 8] % 100) % 10;
					debugPC += 2;
				break;

				case 0x0055: // FX55: Stores V0 to VX in debugMemory starting at address debugI
					for (int i = 0; i <= ((debugOpcode & 0x0F00) >> 8); ++i)
						debugMemory[debugI + i] = V[i];

					// On the original interpreter, when the operation is done, debugI = debugI + X + 1.
					debugI += ((debugOpcode & 0x0F00) >> 8) + 1;
					debugPC += 2;
				break;

				case 0x0065: // FX65: Fills V0 to VX with values from debugMemory starting at address debugI
					for (int i = 0; i <= ((debugOpcode & 0x0F00) >> 8); ++i)
						V[i] = debugMemory[debugI + i];

					// On the original interpreter, when the operation is done, debugI = debugI + X + 1.
					debugI += ((debugOpcode & 0x0F00) >> 8) + 1;
					debugPC += 2;
				break;

				default:
					printf ("Unknown debugOpcode [0xF000]: 0x%X\n", debugOpcode);
			}
		break;

		default:
			printf ("Unknown debugOpcode: 0x%X\n", debugOpcode);
	}

	// Update timers
	if(debugDelay_timer > 0)
		--debugDelay_timer;

	if(debugSound_timer > 0)
	{
		if(debugSound_timer == 1)
			printf("BEEP!\n");
		--debugSound_timer;
	}
	return debugOpcode;
}

bool debug_loadApplication(const char * filename){
	debug_init();
	printf("Loading: %s\n", filename);

	// Open file
	FILE * pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		fputs ("File error", stderr);
		return false;
	}

	// Check file size
	fseek(pFile , 0 , SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);
	printf("Filesize: %d\n", (int)lSize);

	// Allocate debugMemory to contain the whole file
	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		fputs ("Memory error", stderr);
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread (buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error",stderr);
		return false;
	}

	// Copy buffer to Chip8 debugMemory
	if((4096-512) > lSize)
	{
		for(int i = 0; i < lSize; ++i)
			debugMemory[i + 512] = buffer[i];
	}
	else
		printf("Error: ROM too big for debugMemory");

	// Close file, free buffer
	fclose(pFile);
	free(buffer);

	return true;
}
