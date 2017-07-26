#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include <stdbool.h>
#include "debug.h"

#include <GL/glut.h>
// http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/

#define MODIFIER 10
#define DISPLAY_WIDTH SCREEN_WIDTH * MODIFIER
#define DISPLAY_HEIGHT SCREEN_HEIGHT * MODIFIER

void setUpOpenGL();
void updateScreen();
void display();
bool checkReg();

bool checkReg(){
	for(int i = 0; i < 16; ++i){
		if(v[i] != V[i])
			return false;
	}
	return true;
}

int main(int argc, char **argv) {
	char* filename = "Z:\\Programas\\IDE\\Eclipse\\Workspace\\Chip-8_Emulator_PC\\Chip-8 Emulator\\src\\games\\pong2.c8";
	//Z:\Programas\IDE\Eclipse\Workspace\Chip-8_Emulator_PC\Chip-8 Emulator\src\games
	printf("Pong \n");
	cpu_loadRom(filename);
	if(!debug_loadApplication(filename)){
		printf("debugLoad Falhou");
		return 0;
	}
	unsigned short prevOp;
	unsigned short prevDebugOp;
	unsigned short op;
	unsigned short debugOp;
	int i = 0;

	/*
	while(true){
		i++;
		prevOp = op;
		prevDebugOp = debugOp;
		op = emulateCycle();
		debugOp = debug_emulateCycle();
		if(op != debugOp){
			printf("%d Op: %x   DebugOP: %x\n", i, op, debugOp);
			break;
		}
		if(!checkReg()){
			printf("%d\n ", i);
			break;
		}
	}
	*/

	while(true){
		i++;
		prevOp = op;
		prevDebugOp = debugOp;
		op = emulateCycle();
		debugOp = debug_emulateCycle();
		if(op != debugOp){
			printf("%d Op: %x   DebugOP: %x\n", i, op, debugOp);
			break;
		}
	}

	printf("%d Op: %x   DebugOP: %x\n", i, op, debugOp);
	printf("prevOp %x\n", prevOp);
	printf("prevDebugOp %x\n", prevDebugOp);

	// Open GL
	/*
	glutInit(&argc, argv);
	setUpOpenGL();

	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutMainLoop();
	*/
	/*
	filename = "C:\\Users\\caiox\\git\\Chip-8 Emulator\\src\\games\\invaders.c8";
	printf("Invaders \n");
	cpu_loadRom(filename);

	filename = "C:\\Users\\caiox\\git\\Chip-8 Emulator\\src\\games\\tetris.c8";
	printf("Invaders \n");
	cpu_loadRom(filename);
	*/
	return 0;
}

void setUpOpenGL(){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Chip-8");
}

void drawPixel(int x, int y){
	glBegin(GL_QUADS);
		glVertex3f((x * MODIFIER) + 0.0f,     (y * MODIFIER) + 0.0f,	 0.0f);
		glVertex3f((x * MODIFIER) + 0.0f,     (y * MODIFIER) + MODIFIER, 0.0f);
		glVertex3f((x * MODIFIER) + MODIFIER, (y * MODIFIER) + MODIFIER, 0.0f);
		glVertex3f((x * MODIFIER) + MODIFIER, (y * MODIFIER) + 0.0f,	 0.0f);
	glEnd();
}

void updateScreen(){
	// Draw
	for(int y = 0; y < SCREEN_HEIGHT; ++y)
		for(int x = 0; x < SCREEN_WIDTH; ++x)
		{
			if(gfx[(y * SCREEN_WIDTH) + x] == 0)
				glColor3f(0.0f,0.0f,0.0f);
			else
				glColor3f(1.0f,1.0f,1.0f);

			drawPixel(x, y);
		}
}

void display(){
	emulateCycle();

	if(drawFlag){
		glClear(GL_COLOR_BUFFER_BIT);
	}

	updateScreen();

	glutSwapBuffers();
	drawFlag = false;
}

void keyboardDown(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		exit(0);

	if(key == '1')		keypad[0x1] = 1;
	else if(key == '2')	keypad[0x2] = 1;
	else if(key == '3')	keypad[0x3] = 1;
	else if(key == '4')	keypad[0xC] = 1;

	else if(key == 'q')	keypad[0x4] = 1;
	else if(key == 'w')	keypad[0x5] = 1;
	else if(key == 'e')	keypad[0x6] = 1;
	else if(key == 'r')	keypad[0xD] = 1;

	else if(key == 'a')	keypad[0x7] = 1;
	else if(key == 's')	keypad[0x8] = 1;
	else if(key == 'd')	keypad[0x9] = 1;
	else if(key == 'f')	keypad[0xE] = 1;

	else if(key == 'z')	keypad[0xA] = 1;
	else if(key == 'x')	keypad[0x0] = 1;
	else if(key == 'c')	keypad[0xB] = 1;
	else if(key == 'v')	keypad[0xF] = 1;

	//printf("Press key %c\n", key);
}

void keyboardUp(unsigned char key, int x, int y)
{
	if(key == '1')		keypad[0x1] = 0;
	else if(key == '2')	keypad[0x2] = 0;
	else if(key == '3')	keypad[0x3] = 0;
	else if(key == '4')	keypad[0xC] = 0;

	else if(key == 'q')	keypad[0x4] = 0;
	else if(key == 'w')	keypad[0x5] = 0;
	else if(key == 'e')	keypad[0x6] = 0;
	else if(key == 'r')	keypad[0xD] = 0;

	else if(key == 'a')	keypad[0x7] = 0;
	else if(key == 's')	keypad[0x8] = 0;
	else if(key == 'd')	keypad[0x9] = 0;
	else if(key == 'f')	keypad[0xE] = 0;

	else if(key == 'z')	keypad[0xA] = 0;
	else if(key == 'x')	keypad[0x0] = 0;
	else if(key == 'c')	keypad[0xB] = 0;
	else if(key == 'v')	keypad[0xF] = 0;
}
