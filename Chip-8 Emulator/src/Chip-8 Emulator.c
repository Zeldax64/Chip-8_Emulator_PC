#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include <stdbool.h>
// http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/

int main(void) {
	char* filename = "C:\\Users\\caiox\\git\\Chip-8 Emulator\\src\\games\\pong2.c8";
	//char* filename = "C:\\oi.txt";
	printf("Pong \n");
	cpu_loadRom(filename);

	/*
	filename = "C:\\Users\\caiox\\git\\Chip-8 Emulator\\src\\games\\invaders.c8";
	printf("Invaders \n");
	cpu_loadRom(filename);

	filename = "C:\\Users\\caiox\\git\\Chip-8 Emulator\\src\\games\\tetris.c8";
	printf("Invaders \n");
	cpu_loadRom(filename);
	*/
}

void keyboardDown(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		exit(0);

	if(key == '1')		key[0x1] = 1;
	else if(key == '2')	key[0x2] = 1;
	else if(key == '3')	key[0x3] = 1;
	else if(key == '4')	key[0xC] = 1;

	else if(key == 'q')	key[0x4] = 1;
	else if(key == 'w')	key[0x5] = 1;
	else if(key == 'e')	key[0x6] = 1;
	else if(key == 'r')	key[0xD] = 1;

	else if(key == 'a')	key[0x7] = 1;
	else if(key == 's')	key[0x8] = 1;
	else if(key == 'd')	key[0x9] = 1;
	else if(key == 'f')	key[0xE] = 1;

	else if(key == 'z')	key[0xA] = 1;
	else if(key == 'x')	key[0x0] = 1;
	else if(key == 'c')	key[0xB] = 1;
	else if(key == 'v')	key[0xF] = 1;

	//printf("Press key %c\n", key);
}

void keyboardUp(unsigned char key, int x, int y)
{
	if(key == '1')		key[0x1] = 0;
	else if(key == '2')	key[0x2] = 0;
	else if(key == '3')	key[0x3] = 0;
	else if(key == '4')	key[0xC] = 0;

	else if(key == 'q')	key[0x4] = 0;
	else if(key == 'w')	key[0x5] = 0;
	else if(key == 'e')	key[0x6] = 0;
	else if(key == 'r')	key[0xD] = 0;

	else if(key == 'a')	key[0x7] = 0;
	else if(key == 's')	key[0x8] = 0;
	else if(key == 'd')	key[0x9] = 0;
	else if(key == 'f')	key[0xE] = 0;

	else if(key == 'z')	key[0xA] = 0;
	else if(key == 'x')	key[0x0] = 0;
	else if(key == 'c')	key[0xB] = 0;
	else if(key == 'v')	key[0xF] = 0;
}
